#include <QDateTime>

#include "utils/convert.h"

#include "filecopier.h"

FileCopier::FileCopier(QString from, QString to) {
    m_sourceFile = new QFile(from);
    m_targetFile = new QFile(to);

    m_fileSize = m_sourceFile->size();
}

FileCopier::~FileCopier() {
    delete m_sourceFile;
    delete m_targetFile;
}

bool FileCopier::copy() {
    m_sourceFile->open(QFile::ReadOnly);
    m_targetFile->open(QFile::WriteOnly);

    qint64 nextBytesUpdate = m_bytesProgressInterval;
    qint64 nextTimeUpdate = now() + m_timeProgressInterval;

    char buffer[1048576];
    m_sourceFile->reset();
    updateProgress();

    while (!m_sourceFile->atEnd()) {
        qint64 read = m_sourceFile->read(buffer, sizeof(buffer));
        if (read <= 0) {
            qWarning("Read Size should be more than zero: " + Convert::toString(read));
        }

        qint64 wrote = m_targetFile->write(buffer, read);
        if (wrote != read) {
            qWarning("Copy chunk error, read: " + Convert::toString(read) + ", wrote: " + Convert::toString(wrote));
            return false;
        }

        m_totalBytesCopied += wrote;

        if (m_bytesProgressInterval > 0 && m_totalBytesCopied >= nextBytesUpdate) {
            updateProgress();
            nextBytesUpdate += m_bytesProgressInterval;
        }

        if (m_timeProgressInterval > 0 && now() >= nextTimeUpdate) {
            updateProgress();
            nextTimeUpdate += m_timeProgressInterval;
        }
    }

    copyFileTimes();

    m_sourceFile->close();
    m_targetFile->close();

    updateProgress();

    return true;
}

qint64 FileCopier::now() {
    return QDateTime::currentMSecsSinceEpoch();
}

void FileCopier::updateProgress() {
    int filePercentage = 100 * m_totalBytesCopied / m_fileSize;
    qint64 bytesSinceLastUpdate = m_totalBytesCopied - m_bytesOnLastUpdate;
    m_bytesOnLastUpdate = m_totalBytesCopied;
    emit updateProgressSignal(filePercentage, bytesSinceLastUpdate);
}

void FileCopier::copyFileTimes() {
    m_targetFile->setFileTime(m_sourceFile->fileTime(QFileDevice::FileTime::FileBirthTime), QFileDevice::FileTime::FileBirthTime);
    m_targetFile->setFileTime(m_sourceFile->fileTime(QFileDevice::FileTime::FileModificationTime), QFileDevice::FileTime::FileModificationTime);
    m_targetFile->setFileTime(m_sourceFile->fileTime(QFileDevice::FileTime::FileAccessTime), QFileDevice::FileTime::FileAccessTime);
    m_targetFile->setFileTime(m_sourceFile->fileTime(QFileDevice::FileTime::FileMetadataChangeTime), QFileDevice::FileTime::FileMetadataChangeTime);
}
