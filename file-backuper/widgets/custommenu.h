#ifndef CUSTOMMENU_H
#define CUSTOMMENU_H

#include <QMenu>

#include "widgets/customtablewidget.h"

class CustomMenu : public QMenu {
public:
    CustomMenu(CustomTableWidget *table);

private slots:
    void closeEvent(QCloseEvent *event) override;

private:
    CustomTableWidget *m_table;
};

#endif // CUSTOMMENU_H
