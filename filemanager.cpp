#include "filemanager.h"
#include "logger.h"
#include <QCoreApplication>
#include <algorithm>

FileManager::FileManager(QObject* parent)
    : QObject(parent)
{
    m_timer.setInterval(100);
    connect(&m_timer, &QTimer::timeout, this, &FileManager::checkAllFiles);

    Logger::instance().info("Менеджер файлов инициализирован");
}

FileManager::~FileManager()
{
    stopMonitoring();

    // Очищ вектор с наблюдателями
    qDeleteAll(m_watchers);
    m_watchers.clear();

    Logger::instance().info("Менеджер файлов завершил работу");
}
void FileManager::addFile(const QString& filePath)
{
    // Проверка не наблюдается ли уже за этим файлом
    for (FileWatcher* watcher : m_watchers) {
        if (watcher->getFilePath() == filePath) {
            Logger::instance().error(QString("Файл '%1' уже находится под наблюдением").arg(filePath));
            return;
        }
    }

    // Создаем новый наблюдатель (автоматически удалит(( parent = this))
    FileWatcher* watcher = new FileWatcher(filePath, this);

    connect(watcher, &FileWatcher::fileCreated, this,
            [](const QString& path, qint64 size) {
                if (size > 0) {
                    Logger::instance().event(QString("СОБЫТИЕ: Файл '%1' создан, размер: %2 байт")
                                 .arg(path).arg(size));
                } else {
                    Logger::instance().event(QString("СОБЫТИЕ: Файл '%1' создан (пустой)").arg(path));
                }
            });

    connect(watcher, &FileWatcher::fileDeleted, this,
            [](const QString& path) {
                Logger::instance().event(QString("СОБЫТИЕ: Файл '%1' удален или переименован").arg(path));
            });

    connect(watcher, &FileWatcher::fileChanged, this,
            [](const QString& path, qint64 newSize) {
                Logger::instance().event(QString("СОБЫТИЕ: Файл '%1' изменен, новый размер: %2 байт")
                             .arg(path).arg(newSize));
            });

    connect(watcher, &FileWatcher::fileRestored, this,
            [](const QString& path, qint64 size) {
                if (size > 0) {
                    Logger::instance().event(QString("СОБЫТИЕ: Файл '%1' восстановлен, размер: %2 байт")
                                 .arg(path).arg(size));
                } else {
                    Logger::instance().event(QString("СОБЫТИЕ: Файл '%1' восстановлен (пустой)").arg(path));
                }
            });

    m_watchers.append(watcher);

    Logger::instance().info(QString("Файл '%1' добавлен под наблюдение").arg(filePath));
}

void FileManager::removeFile(const QString& filePath)
{
    for (int i = 0; i < m_watchers.size(); ++i) {
        if (m_watchers[i]->getFilePath() == filePath) {
            // Удаляеца наблюдатель
            m_watchers[i]->deleteLater();
            m_watchers.removeAt(i);

            Logger::instance().info(QString("Файл '%1' удален из наблюдения").arg(filePath));
            return;
        }
    }

    Logger::instance().error(QString("Файл '%1' не найден среди наблюдаемых").arg(filePath));
}

void FileManager::printAllFilesInfo() const
{
    Logger::instance().info("=== ТЕКУЩЕЕ СОСТОЯНИЕ НАБЛЮДАЕМЫХ ФАЙЛОВ ===");
    if (m_watchers.isEmpty()) {
        Logger::instance().info("Нет файлов под наблюдением");
    } else {
        for (FileWatcher* watcher : m_watchers) {
            if (watcher->isFileExists()) {
                qint64 size = watcher->getFileSize();
                if (size > 0) {
                    Logger::instance().info(QString("  %1 - существует, размер: %2 байт")
                                .arg(watcher->getFilePath()).arg(size));
                } else {
                    Logger::instance().info(QString("  %1 - существует (пустой)")
                                .arg(watcher->getFilePath()));
                }
            } else {
                Logger::instance().info(QString("  %1 - НЕ существует").arg(watcher->getFilePath()));
            }
        }
    }
    Logger::instance().info("=============================================");
}

void FileManager::startMonitoring(int intervalMs)
{
    if (!m_timer.isActive()) {
        m_timer.setInterval(intervalMs);
        m_timer.start();
        Logger::instance().info(QString("Мониторинг запущен с интервалом %1 мс").arg(intervalMs));

        printAllFilesInfo();
    }
}

void FileManager::stopMonitoring()
{
    if (m_timer.isActive()) {
        m_timer.stop();
        Logger::instance().info("Мониторинг остановлен");
    }
}

void FileManager::checkAllFiles()
{
    for (FileWatcher* watcher : m_watchers) {
        watcher->checkFile();
    }
}


