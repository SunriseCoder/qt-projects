#include <QFile>
#include <QJsonDocument>
#include <QStringLiteral>
#include <QTextStream>

#include "jsonhelper.h"

JsonHelper::JsonHelper() {

}

bool JsonHelper::loadJson(QString filename, QJsonDocument* jsonDoc) {
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open Tasks file");
        return false;
    }

    QByteArray fileData = loadFile.readAll();
    *jsonDoc = QJsonDocument::fromJson(fileData);

    if (!jsonDoc) {
        qWarning("Couldn't load Json Document from Tasks file");
    }

    return jsonDoc;
}
