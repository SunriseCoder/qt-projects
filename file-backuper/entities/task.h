#ifndef TASK_H
#define TASK_H

#include <QJsonObject>
#include <QString>

class Task {

    public:
        Task();

        // JSON Methods
        static Task fromJson(QJsonObject jsonObj);
        void toJson();

        // Getters and Setters
        QString name() { return _name; }
        void setName(QString name) { _name = name; }
    private:
        QString _name;
};

#endif // TASK_H
