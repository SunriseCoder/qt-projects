#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QJsonDocument>

class JsonHelper {
public:
    JsonHelper();

    static bool loadJson(QString filename, QJsonDocument* jsonDoc);
};

#endif // JSONHELPER_H
