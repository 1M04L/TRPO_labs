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
