#include "alltasksexecutor.h"

AllTasksExecutor::AllTasksExecutor(QList<TaskEntity*> *tasks) {
    m_tasks = tasks;
}

void AllTasksExecutor::execute() {
    // Create Task Executors
    m_taskExecutors->clear();
    for (int i = 0; i < m_tasks->size(); i++) {
        TaskEntity *task = m_tasks->at(i);
        TaskExecutor *executor = new TaskExecutor(task);
        connect(executor, &TaskExecutor::needConfirmation, this, &AllTasksExecutor::addConfirmation);
        connect(executor, &TaskExecutor::bytesCopied, this, &AllTasksExecutor::updateProgress);
        m_taskExecutors->insert(task, executor);
    }

    // Scan files for all Tasks
    long totalFileSize = 0;
    for (int i = 0; i < m_tasks->size(); i++) {
        TaskEntity *task = m_tasks->at(i);
        TaskExecutor *executor = m_taskExecutors->value(task);
        long taskFileSize = executor->scan();
        totalFileSize += taskFileSize;
    }

    // Perform backup Tasks
    for (int i = 0; i < m_tasks->size(); i++) {
        TaskEntity *task = m_tasks->at(i);
        TaskExecutor *executor = m_taskExecutors->value(task);
        executor->backup();
    }
}

void AllTasksExecutor::sendAnswer(TaskEntity *task, Question *question) {
    TaskExecutor *executor = m_taskExecutors->value(task);
    executor->sendAnswer(question);
}

void AllTasksExecutor::addConfirmation(Question *question) {
    emit needConfirmation(question);
}

void AllTasksExecutor::updateProgress(qint64 copied, qint64 total) {
    emit bytesCopied(copied, total);
}
