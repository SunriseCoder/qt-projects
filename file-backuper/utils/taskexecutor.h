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

    qint64 bytesCopied() { return m_bytesCopied; }
    qint64 bytesTotal() { return m_bytesTotal; }

signals:
    void needConfirmation(Question *question);
    void updateProgressSignal(int filePercentage, int taskPercentage);

private:
    bool m_backupInProgress = false;
    TaskEntity *m_task;
    QList<FileEntity*> *m_copyQueue = new QList<FileEntity*>();
    QList<FileEntity*> *m_deleteQueue = new QList<FileEntity*>();

    qint64 m_bytesCopied = 0;
    qint64 m_bytesTotal = 0;

    void scanRecursively(QFileInfo *path, QMap<QString, FileEntity*> *fileMap, QFileInfo *startPath);
    QString makeAbsolutePath(QString path1, QString path2);
    bool copyFile(QString sourcePath, QString targetPath);

private slots:
    void updateProgress(int filePercentage, qint64 bytesDelta);
};

#endif // TASKEXECUTOR_H
