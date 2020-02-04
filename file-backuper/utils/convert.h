#ifndef CONVERT_H
#define CONVERT_H

#include <QString>

class Convert {
public:
    Convert();

    static QByteArray toString(qint64);
};

#endif // CONVERT_H
