#include "filewatcher.h"
#include "logger.h"

FileWatcher::FileWatcher(const QString& filePath, QObject* parent)
    : QObject(parent)
    , m_filePath(filePath)
    , m_fileInfo(filePath)
    , m_lastExists(false)
    , m_lastSize(0)
{

    m_lastExists = m_fileInfo.exists() && m_fileInfo.isFile();
    if (m_lastExists) {
        m_lastSize = m_fileInfo.size();
    }

    Logger::instance().info(QString("Добавлен файл для наблюдения: %1").arg(filePath));

    if (m_lastExists) {
        if (m_lastSize > 0) {
            Logger::instance().event(QString("Файл '%1' существует размер: %2 байт")
                         .arg(m_filePath).arg(m_lastSize));
        } else {
            Logger::instance().event(QString("Файл '%1' существует (пустой)").arg(m_filePath));
        }
    } else {
        Logger::instance().event(QString("Файла '%1' не существует").arg(m_filePath));
    }
}

FileWatcher::~FileWatcher()
{
    Logger::instance().info(QString("Прекращено наблюдение за файлом: %1").arg(m_filePath));
}

QString FileWatcher::getFilePath() const
{
    return m_filePath;
}

bool FileWatcher::isFileExists() const
{
    return m_lastExists;
}

qint64 FileWatcher::getFileSize() const
{
    return m_lastSize;
}

bool FileWatcher::updateFileInfo()
{
    m_fileInfo.refresh();

    bool exists = m_fileInfo.exists() && m_fileInfo.isFile();
    qint64 size = exists ? m_fileInfo.size() : 0;

    // Проверяем были ли изменения
    bool changed = (exists != m_lastExists) || (exists && size != m_lastSize);

    if (changed) {
        processChanges(exists, size);
        m_lastExists = exists;
        m_lastSize = size;
    }

    return changed;
}

void FileWatcher::processChanges(bool exists, qint64 size)
{
    if (exists && !m_lastExists) {
        // Файл был создан или восстановлен
        if (size > 0) {
            emit fileCreated(m_filePath, size);
        } else {
            emit fileCreated(m_filePath, 0);
        }
    }
    else if (!exists && m_lastExists) {
        // Файл был удален или переименован
        emit fileDeleted(m_filePath);
    }
    else if (exists && m_lastExists && size != m_lastSize) {
        // Файл размер изменился
        emit fileChanged(m_filePath, size);
    }
}

void FileWatcher::checkFile()
{
    updateFileInfo();
}
