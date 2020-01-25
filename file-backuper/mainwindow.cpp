#include <QJsonArray>
#include <QJsonObject>

#include "entities/task.h"

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

    QJsonArray jsonTasks = jsonDoc.array();
    for (int i = 0; i < jsonTasks.size(); i++) {
        QJsonObject jsonTask = jsonTasks[i].toObject();
        //Task task = Task::fromJSON(jsonTask);
        Task task = Task::fromJson(jsonTask);
        qInfo("Task \"" + task.name().toUtf8() + "\" is loaded");
    }

    return true;
}

MainWindow::~MainWindow() {
    delete ui;
}
