#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractTableModel>

#include "entities/taskentity.h"

class TaskModel : public QAbstractTableModel {
    public:
        TaskModel(QObject *parent, QList<TaskEntity*> *tasks);
        ~TaskModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    private:
        QList<TaskEntity*> *m_tasks;
        QList<QString> *m_tableCaptions = new QList<QString>();
};

#endif // TASKMODEL_H
