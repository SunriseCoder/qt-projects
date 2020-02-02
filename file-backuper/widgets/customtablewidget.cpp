#include "customtablewidget.h"

CustomTableWidget::CustomTableWidget(TaskEntity *task) {
    m_task = task;
}

void CustomTableWidget::contextMenuEvent(QContextMenuEvent *event) {
    emit createContextMenu(this, event);
}
