#ifndef ALLTASKSEXECUTOR_H
#define ALLTASKSEXECUTOR_H

#include <QMap>
#include <QThread>

#include "entities/taskentity.h"

#include "utils/taskexecutor.h"

class AllTasksExecutor : public QObject {
    Q_OBJECT
public:
    AllTasksExecutor(QList<TaskEntity*> *tasks);

    void execute();
    void sendAnswer(TaskEntity *task, Question *question);

signals:
    void needConfirmation(Question *question);
    void updateProgressSignal(int filePercentage, int taskPercentage, int totalPercentage);
    void allTasksFinished(bool succeed);

public slots:
    void addConfirmation(Question *question);
    void updateProgress(int filePercentage, int taskPercentage);

private:
    QList<TaskEntity*> *m_tasks;
    QMap<TaskEntity*, TaskExecutor*> *m_taskExecutors = new QMap<TaskEntity*, TaskExecutor*>();
};

#endif // ALLTASKSEXECUTOR_H
