#include <QJsonArray>
#include <QJsonObject>

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

    ui->taskTable->setModel(taskModel);
    ui->taskTable->horizontalHeader()->setVisible(true);
    ui->taskTable->show();
}

MainWindow::~MainWindow() {
    delete ui;
}
