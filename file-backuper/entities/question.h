#ifndef QUESTION_H
#define QUESTION_H

#include <QString>

#include "entities/fileentity.h"
#include "entities/taskentity.h"

class Question {
public:
    enum Objectives {
        DifferentSizes,
        DifferentModificationTime,
        DeletedSourceFile
    };

    static QString objectiveText(Objectives objective) {
        switch (objective) {
            case DifferentSizes: return "Size";
            case DifferentModificationTime: return "Modification";
            case DeletedSourceFile: return "No Source";
            default: throw std::exception();
        }
    };

    enum Actions {
        NoAction,
        Overwrite,
        Delete,
        Skip
    };

    static QString actionText(Actions action) {
        switch (action) {
            case NoAction: return "No Action";
            case Overwrite: return "Overwrite";
            case Delete: return "Delete";
            case Skip: return "Skip";
            default: throw std::exception();
        }
    }

    Question(TaskEntity *task, FileEntity *sourceFile, FileEntity *targetFile);

    TaskEntity *task() { return m_task; };
    FileEntity *sourceFile() { return m_sourceFile; }
    FileEntity *targetFile() { return m_targetFile; }
    Actions answer() { return m_answer; }
    void setAnswer(Actions answer) { m_answer = answer; }

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
    Actions m_answer = NoAction;
};

#endif // QUESTION_H
