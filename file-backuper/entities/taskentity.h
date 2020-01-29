#ifndef TASK_H
#define TASK_H

#include <QJsonObject>
#include <QString>

class TaskEntity {

    public:
        // JSON Methods
        static TaskEntity fromJson(QJsonObject jsonObj);
        void toJson();

        // Getters and Setters
        QString name() { return m_name; }
        void setName(QString name) { m_name = name; }
    private:
        QString m_name;
};

#endif // TASK_H
