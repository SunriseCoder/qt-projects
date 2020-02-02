#include <QJsonArray>
#include <QJsonObject>

#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QSizePolicy>

#include "entities/taskentity.h"

#include "models/taskmodel.h"

#include "utils/jsonhelper.h"
#include "utils/taskexecutor.h"

#include "widgets/customaction.h"
#include "widgets/custommenu.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    loadData();
}

bool MainWindow::loadData() {
    QJsonDocument jsonDoc;
    if (!JsonHelper::loadJson("tasks.json", &jsonDoc)) {
        return false;
    }

    if (!jsonDoc.isArray()) {
        qWarning("Json Root Element is not an Array");
        return false;
    }

    m_tasks->clear();
    QJsonArray jsonTasks = jsonDoc.array();
    for (int i = 0; i < jsonTasks.size(); i++) {
        QJsonObject jsonTask = jsonTasks[i].toObject();
        TaskEntity *task = TaskEntity::fromJson(jsonTask);
        m_tasks->append(task);
        qInfo("Task \"" + task->name().toUtf8() + "\" is loaded");
    }

    fillTaskTable();
    createTaskTables();

    return true;
}

void MainWindow::fillTaskTable() {
    TaskModel *taskModel = new TaskModel(this, m_tasks);

    // Tasks Label
    QLabel *labelTasks = new QLabel("Tasks:");
    labelTasks->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    ui->scrollAreaLayout->addWidget(labelTasks);

    // Tasks Table
    QTableView *taskTable = new QTableView();
    taskTable->setModel(taskModel);
    taskTable->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    taskTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->scrollAreaLayout->addWidget(taskTable);

    // Stub Label to fill the remaining space in the bottom of the ScrollArea
    QLabel *stubLabel = new QLabel();
    stubLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ui->scrollAreaLayout->addWidget(stubLabel);
}

void MainWindow::createTaskTables() {
    for (int i = 0; i < m_tasks->size(); i++) {
        TaskEntity *task = m_tasks->at(i);

        // Particular Task Label
        QLabel *label = new QLabel(task->name());
        label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        ui->scrollAreaLayout->insertWidget(ui->scrollAreaLayout->count() - 1, label);

        // Particular Task Table
        CustomTableWidget *table = new CustomTableWidget(task);
        table->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        table->setSizeAdjustPolicy(QScrollArea::AdjustToContents);
        table->setColumnCount(4);
        table->setHorizontalHeaderItem(0, new QTableWidgetItem("Source File"));
        table->setHorizontalHeaderItem(1, new QTableWidgetItem("Target File"));
        table->setHorizontalHeaderItem(2, new QTableWidgetItem("Difference"));
        table->setHorizontalHeaderItem(3, new QTableWidgetItem("User Choice"));
        m_taskTables->insert(task, table);

        ui->scrollAreaLayout->insertWidget(ui->scrollAreaLayout->count() - 1, table);
    }
}

void MainWindow::on_startButton_clicked() {
    ui->startButton->setEnabled(false);

    // Clearing Data from previous run
    m_taskExecutors->clear();
    m_questions->clear();

    // Clearing Tables
    QList<CustomTableWidget*> taskTables = m_taskTables->values();
    for (int i = 0; i < taskTables.size(); i++) {
        CustomTableWidget *table = taskTables.at(i);
        table->setRowCount(0);
    }

    // Run Tasks
    executeTasks();
    ui->startButton->setEnabled(true);
}

void MainWindow::executeTasks() {
    // Create Task Executors
    for (int i = 0; i < m_tasks->size(); i++) {
        TaskEntity *task = m_tasks->at(i);
        TaskExecutor *executor = new TaskExecutor(task);
        connect(executor, &TaskExecutor::needConfirmation, this, &MainWindow::addConfirmation);
        m_taskExecutors->insert(task, executor);
    }

    // Scan files for all Tasks
    long totalFileSize = 0;
    for (int i = 0; i < m_tasks->size(); i++) {
        TaskEntity *task = m_tasks->at(i);
        TaskExecutor *executor = m_taskExecutors->value(task);
        long taskFileSize = executor->scan();
        totalFileSize += taskFileSize;
    }

    // Perform backup Tasks
    for (int i = 0; i < m_tasks->size(); i++) {
        TaskEntity *task = m_tasks->at(i);
        TaskExecutor *executor = m_taskExecutors->value(task);
        executor->backup();
    }
}

void MainWindow::addConfirmation(Question *question) {
    // Add Entry to the particular TaskTable
    TaskEntity *task = question->task();
    CustomTableWidget *table = m_taskTables->value(task);

    int rowNumber = table->rowCount();

    QMap<int, Question*> tableQuestions = m_questions->value(task);
    tableQuestions.insert(rowNumber, question);
    m_questions->insert(task, tableQuestions);

    // Insert new Row
    table->insertRow(rowNumber);

    // Source and Target Files
    if (question->sourceFile() != nullptr) {
        table->setItem(rowNumber, 0, new QTableWidgetItem(question->sourceFile()->absolutePath()));
    }
    if (question->targetFile() != nullptr) {
        table->setItem(rowNumber, 1, new QTableWidgetItem(question->targetFile()->absolutePath()));
    }

    // Objectives
    QList<Question::Objectives> *objectives = question->objectives();
    QString objectivesText = "";
    for (int i = 0; i < objectives->size(); i++) {
        if (!objectivesText.isEmpty()) {
            objectivesText += ", ";
        }
        Question::Objectives objective = objectives->at(i);
        objectivesText += Question::objectiveText(objective);
    }
    table->setItem(rowNumber, 2, new QTableWidgetItem(objectivesText));

    // TODO Find out automatic way to resize columns (probably table full-width use)
    table->resizeColumnsToContents();

    // Table ContextMenu
    table->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(table, &CustomTableWidget::createContextMenu, this, &MainWindow::handleTableContextMenu);
}

void MainWindow::handleTableContextMenu(CustomTableWidget *table, QContextMenuEvent *event) {
    if (table->contextMenuLock()) {
        return;
    }

    QItemSelectionModel *selection = table->selectionModel();
    if (!selection->hasSelection()) {
        return;
    }

    QModelIndexList selectedRows = selection->selectedRows();
    if (selectedRows.size() < 1) {
        return;
    }

    // Validate Table Selection
    int firstRowIndex = selectedRows.first().row();
    TaskEntity *task = table->task();
    QMap<int, Question*> tableQuestions = m_questions->value(task);
    Question *firstQuestion = tableQuestions.value(firstRowIndex);
    QList<Question::Objectives> *firstObjectives = firstQuestion->objectives();
    for (int i = 1; i < selectedRows.size(); i++) {
        int rowNumber = selectedRows.at(i).row();
        Question *question = tableQuestions.value(rowNumber);
        QList<Question::Objectives> *objectives = question->objectives();
        if (*objectives != *firstObjectives) {
            qWarning("No ContextMenu created due to different types Row selected");
            return;
        }
    }

    qInfo("Creating ContextMenu for task: " + table->task()->name().toUtf8());
    table->setContextMenuLock(true);

    CustomMenu *menu = new CustomMenu(table);

    QList<Question::Actions> *actions = firstQuestion->actions();
    for (int i = 0; i < actions->size(); i++) {
        Question::Actions action = actions->at(i);
        QString actionText = Question::actionText(action);
        CustomAction *menuAction = new CustomAction(actionText, action, task, this);
        menu->addAction(menuAction);
        connect(menuAction, &CustomAction::actionClicked, this, &MainWindow::processUserAnswer);
    }
    menu->popup(event->globalPos());
}

void MainWindow::processUserAnswer(TaskEntity *task, Question::Actions action) {
    // Apply the Answer to the whole Table Selection,
    // which was Validated before Context Menu creation
    CustomTableWidget *table = m_taskTables->value(task);
    QItemSelectionModel *selection = table->selectionModel();
    if (!selection->hasSelection()) {
        return;
    }

    QModelIndexList selectedRows = selection->selectedRows();
    if (selectedRows.size() < 1) {
        return;
    }

    CustomTableWidget *questionsTable = m_taskTables->value(task);
    TaskExecutor *executor = m_taskExecutors->value(task);
    QMap<int, Question*> tableQuestionsMap = m_questions->value(task);
    for (int i = 0; i < selectedRows.size(); i++) {
        int rowNumber = selectedRows.at(i).row();

        // Propogation Answer to the Table
        QString answerText = Question::actionText(action);
        questionsTable->setItem(rowNumber, 3, new QTableWidgetItem(answerText));

        // Propogation Answer to Questions Map
        Question *question = tableQuestionsMap.value(rowNumber);
        question->setAnswer(action);

        // Propogating Answer to Task Executor
        executor->sendAnswer(question);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
