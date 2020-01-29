#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>

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

private:
    Ui::MainWindow *ui;

    QList<TaskEntity> tasks;
    QMap<QString, QTableView> tables;

    bool loadData();
    void fillTaskTable();
};
#endif // MAINWINDOW_H
