#include <QDir>
#include <QMap>
#include <QMapIterator>
#include <QSet>

#include "utils/convert.h"
#include "utils/filecopier.h"

#include "taskexecutor.h"

TaskExecutor::TaskExecutor(TaskEntity *task) {
    m_task = task;
}

long TaskExecutor::scan() {
    // Scan Source
    QMap<QString, FileEntity*> *sourceFilesMap = new QMap<QString, FileEntity*>();
    QFileInfo *sourceStartFolder = new QFileInfo(m_task->from());
    sourceStartFolder->makeAbsolute();
    scanRecursively(sourceStartFolder, sourceFilesMap, sourceStartFolder);

    // Scan Target
    QMap<QString, FileEntity*> *targetFilesMap = new QMap<QString, FileEntity*>();
    QFileInfo *targetStartFolder = new QFileInfo(m_task->to());
    targetStartFolder->makeAbsolute();
    scanRecursively(targetStartFolder, targetFilesMap, targetStartFolder);

    // Comparing Source Files against Target Files
    QListIterator<FileEntity*> *sourceFilesIterator = new QListIterator<FileEntity*>(sourceFilesMap->values());
    while (sourceFilesIterator->hasNext()) {
        FileEntity *sourceFile = sourceFilesIterator->next();
        if (targetFilesMap->contains(sourceFile->relativePath())) {
            FileEntity *targetFile = targetFilesMap->value(sourceFile->relativePath());
            Question *question = nullptr;

            // Different File Sizes
            if (sourceFile->size() != targetFile->size()) {
                if (question == nullptr) {
                    question = new Question(m_task, sourceFile, targetFile);
                }
                question->addObjective(Question::DifferentSizes);
            }

            // Different Modified Time
            if (sourceFile->modified() != targetFile->modified()) {
                if (question == nullptr) {
                    question = new Question(m_task, sourceFile, targetFile);
                }
                question->addObjective(Question::DifferentModificationTime);
            }

            // If one of the condition was triggered, sending Question to the User
            if (question != nullptr) {
                // Add possible choice options for the User
                question->addAction(Question::Overwrite);
                question->addAction(Question::Skip);

                // Sending the Question to the User
                emit needConfirmation(question);
            }
        } else {
            // New File - just copy it
            m_copyQueue->append(sourceFile);
            m_bytesTotal += sourceFile->size().toLongLong();
        }
    }

    // Comparing Target Files against Source Files
    QListIterator<FileEntity*> *targetFilesIterator = new QListIterator<FileEntity*>(targetFilesMap->values());
    while (targetFilesIterator->hasNext()) {
        FileEntity *targetFile = targetFilesIterator->next();
        Question *question = nullptr;

        if (!sourceFilesMap->contains(targetFile->relativePath())) {
            // Source File deleted, ask user delete or not
            if (question == nullptr) {
                question = new Question(m_task, nullptr, targetFile);
            }
            question->addObjective(Question::DeletedSourceFile);
        }

        if (question != nullptr) {
            // Add possible choice options for the User
            question->addAction(Question::Delete);
            question->addAction(Question::Skip);

            // Sending the Question to the User
            emit needConfirmation(question);
        }
    }

    // TODO Calculate copy file sizes and return result
    return 0;
}

void TaskExecutor::scanRecursively(QFileInfo *path, QMap<QString, FileEntity*> *fileMap, QFileInfo *startPath) {
    QString relativePath = path->absoluteFilePath().mid(startPath->absoluteFilePath().length() + 1);
    if (m_task->excludePaths()->contains(relativePath)) {
        qDebug("Exclude path: " + path->absoluteFilePath().toUtf8() + " - skipping");
        return;
    }

    if (!path->exists()) {
        qErrnoWarning(-1, "Path not found: " + path->absoluteFilePath().toUtf8());
        return;
    }

    // TODO Follow Symlinks Option Check must be implemented here
    // By default do not follow symlinks
    if (path->isSymLink()) {
        qDebug("Following path is a symlink: " + path->absoluteFilePath().toUtf8() + " - skipping");
        return;
    }

    if (path->isDir()) {
        QDir folder(path->absoluteFilePath());
        QList<QFileInfo> fileList = folder.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        if (fileList.size() == 0) {
            // TODO Empty folder - create another queue and add this folder to that queue
            // Or add it to the fileMap (add isDir flag to FileEntity first) and then just overwrite without prompt
        } else {
            for (int i = 0; i < fileList.size(); i++) {
                QString childPathString = fileList.at(i).absoluteFilePath();
                QFileInfo *childPath = new QFileInfo(childPathString);
                scanRecursively(childPath, fileMap, startPath);
            }
        }
    } else {
        FileEntity *file = new FileEntity(path, startPath);
        fileMap->insert(file->relativePath(), file);
    }
}

void TaskExecutor::sendAnswer(Question *question) {
    Question::Actions action = question->answer();
    if (action == Question::Actions::Overwrite) {
        m_copyQueue->append(question->sourceFile());
        m_bytesTotal += question->sourceFile()->size().toLongLong();
    } else if (action == Question::Actions::Delete) {
        m_deleteQueue->append(question->targetFile());
    } else if (action == Question::Actions::Skip) {
        // Just do nothing
    } else {
        qWarning("Unknown User Answer");
    }

    // Invoking to process file (especially after backup is over)
    backup();
}

void TaskExecutor::backup() {
    // Checking Backup in Progress Lock
    if (m_backupInProgress) {
        return;
    }
    // Acquiring Lock
    m_backupInProgress = true;

    // Actual Files Copy
    qInfo("=== Backup Task: " + m_task->name().toUtf8() + " ===");
    qInfo("-- Copy Files --");
    QMutableListIterator<FileEntity*> copyQueueIterator(*m_copyQueue);
    for (;copyQueueIterator.hasNext();copyQueueIterator.remove()) {
        FileEntity *fileEntity = copyQueueIterator.next();

        QString sourcePath = fileEntity->absolutePath();
        QString targetPath = makeAbsolutePath(m_task->to(), fileEntity->relativePath());

        qInfo(sourcePath.toUtf8() + " -> " + targetPath.toUtf8());
        bool copyResult = copyFile(sourcePath, targetPath);
        if (!copyResult) {
            qWarning("Couldn't copy file \"" + sourcePath.toUtf8() + "\" -> \"" + targetPath.toUtf8() + "\"");
            // TODO Return back to User
        }
    }

    // Delete old files
    qInfo("-- Delete Files --");
    for (int i = 0; i < m_deleteQueue->size(); i++) {
        FileEntity *file = m_deleteQueue->at(i);
        QString filePath = file->absolutePath();

        qInfo(filePath.toUtf8());

        if (QFile::exists(filePath)) {
            QFile::remove(filePath);
        }
    }
    qInfo("=== End of the dump (Task: " + m_task->name().toUtf8() + ") ===");

    // Releasing Lock after backup is done
    m_backupInProgress = false;
}

QString TaskExecutor::makeAbsolutePath(QString path1, QString path2) {
    QFileInfo targetFileInfo(path1);
    targetFileInfo.makeAbsolute();
    QString result = targetFileInfo.absoluteFilePath() + "/" + path2;
    return result;
}

bool TaskExecutor::copyFile(QString sourcePath, QString targetPath) {
    QFileInfo targetFileInfo(targetPath);
    // If File already exists
    if (targetFileInfo.exists()) {
        // Removing
        bool removeResult = QFile::remove(targetPath);
        if (!removeResult) {
            return false;
        }
    } else {
        // Otherwise first trying to create all parent folders
        QFileInfo targetFileParentInfo(targetFileInfo.absolutePath());
        if (!targetFileParentInfo.exists()) {
            QDir targetFileParentDir(targetFileParentInfo.absoluteFilePath());
            bool mkpathResult = targetFileParentDir.mkpath(".");
            if (!mkpathResult) {
                return false;
            }
        }
    }

    // Actual File Copy
    FileCopier copier(sourcePath, targetPath);
    copier.setTimeProgressInterval(100);
    connect(&copier, &FileCopier::updateProgressSignal, this, &TaskExecutor::updateProgress);
    bool copyResult = copier.copy();
    disconnect(&copier, &FileCopier::updateProgressSignal, this, &TaskExecutor::updateProgress);

    return copyResult;
}

void TaskExecutor::updateProgress(int filePercentage, qint64 bytesDelta) {
    m_bytesCopied += bytesDelta;
    int taskPercentage = 100 * m_bytesCopied / m_bytesTotal;
    emit updateProgressSignal(filePercentage, taskPercentage);
}
