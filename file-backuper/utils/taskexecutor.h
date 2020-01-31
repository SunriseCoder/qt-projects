#ifndef TASKEXECUTOR_H
#define TASKEXECUTOR_H

#include <QMap>
#include <QObject>

#include "entities/question.h"
#include "entities/fileentity.h"
#include "entities/taskentity.h"

class TaskExecutor : public QObject {
    Q_OBJECT
public:
    TaskExecutor(TaskEntity *task);

    long scan();

signals:
    void needConfirmation(Question *question);

private:
    TaskEntity *m_task;
    QList<FileEntity*> *copyQueue = new QList<FileEntity*>();
    QList<FileEntity*> *deleteQueue = new QList<FileEntity*>();
    QList<Question*> *questionsQueue = new QList<Question*>();

    void scanRecursively(QFileInfo *path, QMap<QString, FileEntity*> *fileMap, QFileInfo *startPath);
};

#endif // TASKEXECUTOR_H
