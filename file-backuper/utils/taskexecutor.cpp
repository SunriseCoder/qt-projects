#include <QDir>
#include <QMap>
#include <QMapIterator>

#include "taskexecutor.h"

TaskExecutor::TaskExecutor(TaskEntity *task) {
    m_task = task;
}

long TaskExecutor::scan() {
    QMap<QString, FileEntity*> *sourceFilesMap = new QMap<QString, FileEntity*>();
    QFileInfo *sourceStartFolder = new QFileInfo(m_task->from());
    sourceStartFolder->makeAbsolute();
    scanRecursively(sourceStartFolder, sourceFilesMap, sourceStartFolder);

    QMap<QString, FileEntity*> *targetFilesMap = new QMap<QString, FileEntity*>();
    QFileInfo *targetStartFolder = new QFileInfo(m_task->to());
    targetStartFolder->makeAbsolute();
    scanRecursively(targetStartFolder, targetFilesMap, targetStartFolder);

    // Comparing Source Files against Target Files
    //QMapIterator<QString, FileEntity*> *sourceFilesIterator = new QMapIterator<QString, FileEntity*>(sourceFilesMap);
    //QMapIterator<QString, FileEntity*> sourceFilesIterator(sourceFilesMap);
    //sourceFiles->values()
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
                question->addObjective(Question::Objectives::DifferentSizes);
            }

            // Different Creation Time
            if (sourceFile->created() != targetFile->created()) {
                if (question == nullptr) {
                    question = new Question(m_task, sourceFile, targetFile);
                }
                question->addObjective(Question::Objectives::DifferentCreateTime);
            }

            // Different Modified Time
            if (sourceFile->modified() != targetFile->modified()) {
                if (question == nullptr) {
                    question = new Question(m_task, sourceFile, targetFile);
                }
                question->addObjective(Question::Objectives::DifferentModificationTime);
            }

            // If one of the condition was triggered, sending Question to the User
            if (question != nullptr) {
                // Add possible choice options for the User
                question->addAction(Question::Actions::Overwrite);
                question->addAction(Question::Actions::Skip);

                // Saving File Task to the Queue
                questionsQueue->append(question);

                // Sending the Question to the User
                emit needConfirmation(question);
            }
        } else {
            // New File - just copy it
            copyQueue->append(sourceFile);
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
            question->addObjective(Question::Objectives::DeletedSourceFile);
        }

        if (question != nullptr) {
            // Add possible choice options for the User
            question->addAction(Question::Actions::Delete);
            question->addAction(Question::Actions::Skip);

            // Saving File Task to the Queue
            questionsQueue->append(question);

            // Sending the Question to the User
            emit needConfirmation(question);
        }
    }

    // TODO Calculate copy file sizes and return result
    return 0;
}

void TaskExecutor::scanRecursively(QFileInfo *path, QMap<QString, FileEntity*> *fileMap, QFileInfo *startPath) {
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
