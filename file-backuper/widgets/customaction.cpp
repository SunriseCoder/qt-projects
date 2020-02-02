#include "customaction.h"

CustomAction::CustomAction(const QString &text, Question::Actions action, TaskEntity *task, QObject *parent) {
    setText(text);
    setParent(parent);
    m_action = action;
    m_task = task;

    connect(this, &CustomAction::triggered, this, &CustomAction::sendUserAnswer);
}

void CustomAction::sendUserAnswer() {
    emit actionClicked(m_task, m_action);
}
