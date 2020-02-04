#include "fileentity.h"

FileEntity::FileEntity(QFileInfo *fileInfo, QFileInfo *startPath) {
    m_absolutePath = fileInfo->absoluteFilePath();
    m_relativePath = fileInfo->absoluteFilePath().mid(startPath->absoluteFilePath().length() + 1);
    m_size = QString::number(fileInfo->size());
    m_modified = fileInfo->lastModified();
}
