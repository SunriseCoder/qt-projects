#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

#include "entities/question.h"
#include "entities/taskentity.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void addConfirmation(Question *question);

private:
    Ui::MainWindow *ui;

    QList<TaskEntity*> *tasks = new QList<TaskEntity*>();
    QMap<TaskEntity*, QTableWidget*> *tables = new QMap<TaskEntity*, QTableWidget*>();

    bool loadData();
    void fillTaskTable();
    void createTaskTables();
    void executeTasks();
};
#endif // MAINWINDOW_H
