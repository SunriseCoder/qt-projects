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
    void sendAnswer(Question *question);
    void backup();

signals:
    void needConfirmation(Question *question);

private:
    bool m_backupInProgress = false;
    TaskEntity *m_task;
    QList<FileEntity*> *m_copyQueue = new QList<FileEntity*>();
    QList<FileEntity*> *m_deleteQueue = new QList<FileEntity*>();

    void scanRecursively(QFileInfo *path, QMap<QString, FileEntity*> *fileMap, QFileInfo *startPath);
};

#endif // TASKEXECUTOR_H
