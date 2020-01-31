#include <QJsonArray>
#include <QJsonObject>

#include <QSizePolicy>
#include <QTableWidget>

#include "entities/taskentity.h"

#include "models/taskmodel.h"

#include "utils/jsonhelper.h"
#include "utils/taskexecutor.h"

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

    tasks->clear();
    QJsonArray jsonTasks = jsonDoc.array();
    for (int i = 0; i < jsonTasks.size(); i++) {
        QJsonObject jsonTask = jsonTasks[i].toObject();
        TaskEntity *task = TaskEntity::fromJson(jsonTask);
        tasks->append(task);
        qInfo("Task \"" + task->name().toUtf8() + "\" is loaded");
    }

    fillTaskTable();

    return true;
}

void MainWindow::fillTaskTable() {
    TaskModel *taskModel = new TaskModel(this, tasks);

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

void MainWindow::on_startButton_clicked() {
    ui->startButton->setEnabled(false);

    createTaskTables();
    executeTasks();

    ui->startButton->setEnabled(true);
}

void MainWindow::createTaskTables() {
    for (int i = 0; i < tasks->size(); i++) {
        TaskEntity *task = tasks->at(i);

        // Particular Task Label
        QLabel *label = new QLabel(task->name());
        label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        ui->scrollAreaLayout->insertWidget(ui->scrollAreaLayout->count() - 1, label);

        // Particular Task Table
        QTableWidget *table = new QTableWidget();
        table->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        table->setSizeAdjustPolicy(QScrollArea::AdjustToContents);
        table->setColumnCount(4);
        table->setHorizontalHeaderItem(0, new QTableWidgetItem("Source File"));
        table->setHorizontalHeaderItem(1, new QTableWidgetItem("Target File"));
        table->setHorizontalHeaderItem(2, new QTableWidgetItem("Difference"));
        table->setHorizontalHeaderItem(3, new QTableWidgetItem("User Choice"));
        tables->insert(task, table);

        ui->scrollAreaLayout->insertWidget(ui->scrollAreaLayout->count() - 1, table);
    }
}

void MainWindow::executeTasks() {
    QList<TaskExecutor*> *executors = new QList<TaskExecutor*>();

    // Create Task Executors
    for (int i = 0; i < tasks->size(); i++) {
        TaskEntity *task = tasks->at(i);
        TaskExecutor *executor = new TaskExecutor(task);
        connect(executor, &TaskExecutor::needConfirmation, this, &MainWindow::addConfirmation);
        executors->append(executor);
    }

    // Scan files for all Tasks
    long totalFileSize = 0;
    for (int i = 0; i < tasks->size(); i++) {
        TaskExecutor *executor = executors->at(i);
        long taskFileSize = executor->scan();
        totalFileSize += taskFileSize;
    }

    // Perform backup Tasks
    for (int i = 0; i < tasks->size(); i++) {
        TaskExecutor *executor = executors->at(i);
        //executor->backup();
    }
}

void MainWindow::addConfirmation(Question *question) {
    // Add Entry to the particular TaskTable
    QTableWidget *table = tables->value(question->task());

    // Insert new Row
    int rowNumber = table->rowCount();
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
    QString objectivesString = "";
    for (int i = 0; i < objectives->size(); i++) {
        if (!objectivesString.isEmpty()) {
            objectivesString += ", ";
        }

        Question::Objectives objective = objectives->at(i);
        if (objective == Question::Objectives::DifferentSizes) {
            objectivesString += "Size";
        }

        if (objective == Question::Objectives::DifferentCreateTime) {
            objectivesString += "Creation";
        }

        if (objective == Question::Objectives::DifferentModificationTime) {
            objectivesString += "Modification";
        }

        if (objective == Question::Objectives::DeletedSourceFile) {
            objectivesString += "No Source";
        }
    }
    table->setItem(rowNumber, 2, new QTableWidgetItem(objectivesString));

    // TODO Find out automatic way to resize columns (probably table full-width use)
    table->resizeColumnsToContents();
}

MainWindow::~MainWindow() {
    delete ui;
}
