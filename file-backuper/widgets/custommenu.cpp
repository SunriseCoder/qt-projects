#include "custommenu.h"

CustomMenu::CustomMenu(CustomTableWidget *table) {
    m_table = table;
}

void CustomMenu::closeEvent(QCloseEvent *event) {
    m_table->setContextMenuLock(false);
}
