#include "fileentity.h"

FileEntity::FileEntity(QFileInfo *fileInfo, QFileInfo *startPath) {
    m_absolutePath = fileInfo->absoluteFilePath();
    m_relativePath = fileInfo->absoluteFilePath().mid(startPath->absoluteFilePath().length() + 1);
    m_size = fileInfo->size();
    m_created = fileInfo->birthTime();
    m_modified = fileInfo->lastModified();
}
