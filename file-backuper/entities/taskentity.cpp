#include <QJsonArray>
#include <QList>

#include "utils/jsonhelper.h"

#include "taskentity.h"

TaskEntity* TaskEntity::fromJson(QJsonObject jsonObj) {
    TaskEntity *task = new TaskEntity();

    if (jsonObj.contains("name")) {
        QString name = jsonObj["name"].toString();
        task->setName(name);
    }

    if (jsonObj.contains("from")) {
        QString from = jsonObj["from"].toString();
        task->setFrom(from);
    }

    if (jsonObj.contains("to")) {
        QString to = jsonObj["to"].toString();
        task->setTo(to);
    }

    if (jsonObj.contains("excludePaths")) {
        QJsonArray excludePathsArray = jsonObj["excludePaths"].toArray();
        QSet<QString> *excludePaths = new QSet<QString>();
        foreach (QVariant qVariant, excludePathsArray.toVariantList()) {
            excludePaths->insert(qVariant.value<QString>());
        }
        task->setExcludePaths(excludePaths);
    }

    return task;
}
