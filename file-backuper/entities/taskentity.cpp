#include "utils/jsonhelper.h"

#include "taskentity.h"

TaskEntity TaskEntity::fromJson(QJsonObject jsonObj) {
    TaskEntity task;

    if (jsonObj.contains("name")) {
        task.setName(jsonObj["name"].toString());
    }

    return task;
}
