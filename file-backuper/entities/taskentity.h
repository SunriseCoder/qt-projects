#ifndef TASK_H
#define TASK_H

#include <QJsonObject>
#include <QString>

class TaskEntity {
public:
    // JSON Methods
    static TaskEntity* fromJson(QJsonObject jsonObj);
    void toJson();

    // Getters and Setters
    QString name() { return m_name; }
    void setName(QString name) { m_name = name; }
    QString from() { return m_from; }
    void setFrom(QString from) { m_from = from; }
    QString to() { return m_to; }
    void setTo(QString to) { m_to = to; }

private:
    QString m_name;
    QString m_from;
    QString m_to;
};

#endif // TASK_H
