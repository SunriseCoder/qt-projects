#include "question.h"

Question::Question(TaskEntity *task, FileEntity *sourceFile, FileEntity *targetFile) {
    m_task = task;
    m_sourceFile = sourceFile;
    m_targetFile = targetFile;
};

void Question::addObjective(Objectives objective) {
    m_objectives->append(objective);
}

void Question::addAction(Actions action) {
    m_actions->append(action);
}
