#ifndef FILECOPIER_H
#define FILECOPIER_H

#include <QFile>

class FileCopier : public QObject {
    Q_OBJECT
public:
    FileCopier(QString from, QString to);
    ~FileCopier();

    bool copy();

    void setBytesProgressInterval(qint64 bytes) { m_bytesProgressInterval = bytes; }
    void setTimeProgressInterval(qint64 timeInMs) { m_timeProgressInterval = timeInMs; }

signals:
    void bytesCopied(qint64 copied, qint64 total);

private:
    QFile *m_sourceFile;
    QFile *m_targetFile;
    qint64 m_bytesProgressInterval = 0;
    qint64 m_timeProgressInterval = 0;

    qint64 now();
};

#endif // FILECOPIER_H
