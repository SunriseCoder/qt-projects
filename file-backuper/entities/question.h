#ifndef FILEACTIONENTITY_H
#define FILEACTIONENTITY_H

#include <QString>

#include "entities/fileentity.h"
#include "entities/taskentity.h"

class Question {
public:
    enum Objectives {
        DifferentSizes,
        DifferentCreateTime,
        DifferentModificationTime,
        DeletedSourceFile
    };

    enum Actions {
        Skip,
        Overwrite,
        Delete
    };

    Question(TaskEntity *task, FileEntity *sourceFile, FileEntity *targetFile);

    TaskEntity *task() { return m_task; };
    FileEntity *sourceFile() { return m_sourceFile; }
    FileEntity *targetFile() { return m_targetFile; }

    QList<Objectives> *objectives() { return m_objectives; }
    QList<Actions> *actions() { return m_actions; }

    void addObjective(Objectives objective);
    void addAction(Actions action);

private:
    TaskEntity *m_task;
    QList<Objectives> *m_objectives = new QList<Objectives>();
    QList<Actions> *m_actions = new QList<Actions>();
    FileEntity *m_sourceFile;
    FileEntity *m_targetFile;
};

#endif // FILEACTIONENTITY_H
