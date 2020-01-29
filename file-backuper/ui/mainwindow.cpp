#include <QJsonArray>
#include <QJsonObject>

#include <QSizePolicy>
#include <QTableWidget>

#include "entities/taskentity.h"

#include "models/taskmodel.h"

#include "utils/jsonhelper.h"

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

    tasks.clear();
    QJsonArray jsonTasks = jsonDoc.array();
    for (int i = 0; i < jsonTasks.size(); i++) {
        QJsonObject jsonTask = jsonTasks[i].toObject();
        TaskEntity task = TaskEntity::fromJson(jsonTask);
        tasks.append(task);
        qInfo("Task \"" + task.name().toUtf8() + "\" is loaded");
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
    for (int i = 0; i < tasks.size(); i++) {
        TaskEntity task = tasks.at(i);

        // Particular Task Label
        QLabel *label = new QLabel(task.name());
        label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        ui->scrollAreaLayout->insertWidget(ui->scrollAreaLayout->count() - 1, label);

        // Particular Task Table
        QTableWidget *table = new QTableWidget();
        table->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        table->setSizeAdjustPolicy(QScrollArea::AdjustToContents);

        // TODO Remove Debug Data
        for (int j = 0; j < i * 5 + 3; j++) {
            table->insertRow(table->rowCount());
            table->setItem(0, 0, new QTableWidgetItem("Row " + j));
        }

        ui->scrollAreaLayout->insertWidget(ui->scrollAreaLayout->count() - 1, table);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
