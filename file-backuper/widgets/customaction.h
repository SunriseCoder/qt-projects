#ifndef CUSTOMACTION_H
#define CUSTOMACTION_H

#include <QAction>

#include "entities/question.h"

class CustomAction : public QAction {
    Q_OBJECT
public:
    CustomAction(const QString &text, Question::Actions action, TaskEntity *task, QObject *parent = nullptr);

    TaskEntity *task() { return m_task; }

signals:
    void actionClicked(TaskEntity *task, Question::Actions action);

private:
    Question::Actions m_action;
    TaskEntity *m_task;

    void sendUserAnswer();
};

#endif // CUSTOMACTION_H
