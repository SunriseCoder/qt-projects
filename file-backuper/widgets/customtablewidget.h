#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include <QTableWidget>

#include "entities/taskentity.h"

class CustomTableWidget : public QTableWidget {
    Q_OBJECT
public:
    CustomTableWidget(TaskEntity *task);

    TaskEntity *task() { return m_task; }
    bool contextMenuLock() { return m_contextMenuLock; }
    void setContextMenuLock(bool lock) { m_contextMenuLock = lock; }

signals:
    void createContextMenu(CustomTableWidget *table, QContextMenuEvent *event);

private:
    TaskEntity *m_task;
    bool m_contextMenuLock = false;

    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // CUSTOMTABLEWIDGET_H
