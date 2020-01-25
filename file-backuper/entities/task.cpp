#include "utils/jsonhelper.h"

#include "task.h"

Task::Task() {

}

Task Task::fromJson(QJsonObject jsonObj) {
    Task task;

    if (jsonObj.contains("name")) {
        task.setName(jsonObj["name"].toString());
    }

    return task;
}
