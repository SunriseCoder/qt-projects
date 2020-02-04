#include "convert.h"

Convert::Convert() {

}

QByteArray Convert::toString(qint64 number) {
    QString string = QString::number(number);
    return string.toUtf8();
}
