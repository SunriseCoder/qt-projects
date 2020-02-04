#include <QDateTime>

#include "utils/convert.h"

#include "filecopier.h"

FileCopier::FileCopier(QString from, QString to) {
    m_sourceFile = new QFile(from);
    m_targetFile = new QFile(to);
}

FileCopier::~FileCopier() {
    delete m_sourceFile;
    delete m_targetFile;
}

bool FileCopier::copy() {
    m_sourceFile->open(QFile::ReadOnly);
    m_targetFile->open(QFile::WriteOnly);

    qint64 fileSize = m_sourceFile->size();
    qint64 totalCopied = 0;
    qint64 nextBytesUpdate = m_bytesProgressInterval;

    qint64 nextTimeUpdate = now() + m_timeProgressInterval;
    char buffer[1048576];
    m_sourceFile->reset();
    emit bytesCopied(0, fileSize);
    while (!m_sourceFile->atEnd()) {
        qint64 read = m_sourceFile->read(buffer, sizeof(buffer));
        if (read <= 0) {
            qWarning("Read Size should be more than zero: " + Convert::toString(read));
        }

        qint64 write = m_targetFile->write(buffer, read);
        if (write != read) {
            qWarning("Copy chunk error, read: " + Convert::toString(read) + ", wrote: " + Convert::toString(write));
            return false;
        }

        totalCopied += write;

        if (m_bytesProgressInterval > 0 && totalCopied >= nextBytesUpdate) {
            emit bytesCopied(totalCopied, fileSize);
            nextBytesUpdate += m_bytesProgressInterval;
        }

        if (m_timeProgressInterval > 0 && now() >= nextTimeUpdate) {
            emit bytesCopied(totalCopied, fileSize);
            nextTimeUpdate += m_timeProgressInterval;
        }
    }

    emit bytesCopied(totalCopied, fileSize);

    m_sourceFile->close();
    m_targetFile->close();

    return true;
}

qint64 FileCopier::now() {
    return QDateTime::currentMSecsSinceEpoch();
}
