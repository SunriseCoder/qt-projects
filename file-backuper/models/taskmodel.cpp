#include "taskmodel.h"

TaskModel::TaskModel(QObject *parent, QList<TaskEntity> tasks) : QAbstractTableModel(parent) {
    m_tasks = tasks;
    m_tableCaptions << "Name" << "From" << "To" << "Options";
}

int TaskModel::rowCount(const QModelIndex &parent) const {
    int rowCount = m_tasks.size();
    return rowCount;
}

int TaskModel::columnCount(const QModelIndex &parent) const {
    int columnCount = m_tableCaptions.size();
    return columnCount;
}

QVariant TaskModel::data(const QModelIndex &index, int role) const {
    QVariant data;
    if (index.isValid() && role == Qt::DisplayRole) {
        TaskEntity task = m_tasks.at(index.row());
        switch (index.column()) {
            case 0:
                data = task.name();
                break;
        }
    }
    return data;
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section >= 0 && section < m_tableCaptions.size()) {
        QString caption = m_tableCaptions.at(section);
        return caption;
    }
    return QVariant();
}

TaskModel::~TaskModel() {
    // TODO Find out how to delete m_tasks and m_tableCaptions
}
