#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QContextMenuEvent>
#include <QMainWindow>

#include "entities/question.h"
#include "entities/taskentity.h"

#include "utils/taskexecutor.h"

#include "widgets/customtablewidget.h"

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
    void handleTableContextMenu(CustomTableWidget *table, QContextMenuEvent *event);
    void processUserAnswer(TaskEntity *task, Question::Actions action);

private:
    Ui::MainWindow *ui;

    QList<TaskEntity*> *m_tasks = new QList<TaskEntity*>();
    QMap<TaskEntity*, TaskExecutor*> *m_taskExecutors = new QMap<TaskEntity*, TaskExecutor*>();
    QMap<TaskEntity*, CustomTableWidget*> *m_taskTables = new QMap<TaskEntity*, CustomTableWidget*>();
    QMap<TaskEntity*, QMap<int, Question*>> *m_questions = new QMap<TaskEntity*, QMap<int, Question*>>();

    bool loadData();
    void fillTaskTable();
    void createTaskTables();
    void executeTasks();
};
#endif // MAINWINDOW_H
