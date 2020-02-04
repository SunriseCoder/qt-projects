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
    void bytesCopied(qint64 copied, qint64 total);

private:
    bool m_backupInProgress = false;
    TaskEntity *m_task;
    QList<FileEntity*> *m_copyQueue = new QList<FileEntity*>();
    QList<FileEntity*> *m_deleteQueue = new QList<FileEntity*>();

    void scanRecursively(QFileInfo *path, QMap<QString, FileEntity*> *fileMap, QFileInfo *startPath);
    QString makeAbsolutePath(QString path1, QString path2);
    bool copyFile(QString sourcePath, QString targetPath);

private slots:
    void updateProgress(qint64 copied, qint64 total);
};

#endif // TASKEXECUTOR_H
