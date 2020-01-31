#ifndef FILEENTITY_H
#define FILEENTITY_H

#include <QDateTime>
#include <QFileInfo>

class FileEntity {
public:
    FileEntity(QFileInfo *fileInfo, QFileInfo *startPath);

    QString absolutePath() { return m_absolutePath; }
    QString relativePath() { return m_relativePath; }
    long size() { return m_size; }
    QDateTime created() { return m_created; }
    QDateTime modified() { return m_modified; }

private:
    QString m_absolutePath;
    QString m_relativePath;
    long m_size;
    QDateTime m_created;
    QDateTime m_modified;
};

#endif // FILEENTITY_H
