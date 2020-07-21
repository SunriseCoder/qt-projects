#ifndef TASK_H
#define TASK_H

#include <QJsonObject>
#include <QSet>
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
    QSet<QString> *excludePaths() { return m_excludePaths; }
    void setExcludePaths(QSet<QString> *excludePaths) { m_excludePaths = excludePaths; }

private:
    // Constructor
    TaskEntity() {
        m_excludePaths = new QSet<QString>();
    }

    QString m_name;
    QString m_from;
    QString m_to;
    QSet<QString> *m_excludePaths;
};

#endif // TASK_H
