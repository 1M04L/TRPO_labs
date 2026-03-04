#include <QCoreApplication>
#include <QStringList>
#include <QTimer>
#include <iostream>
#include "filemanager.h"
#include "logger.h"

void printUsage()
{
    Logger::instance().info("Использование: FileMonitor <путь_к_файлу1> [путь_к_файлу2] ...");
    Logger::instance().info("Пример: FileMonitor C:\\test.txt D:\\document.txt");
    Logger::instance().info("");
    Logger::instance().info("Программа будет следить за существованием и размером указанных файлов.");
    Logger::instance().info("Для выхода нажмите Ctrl+C");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // экземпляр логгера
    Logger& logger = Logger::instance();

    logger.info("=== ФАЙЛОВЫЙ МОНИТОР ===");

    // Проверка аргументов командной строки
    QStringList args = a.arguments();
    if (args.size() < 2) {
        logger.error("Ошибка: не указаны файлы для наблюдения!");
        printUsage();
        return 1;
    }

    // Удаляем первый аргумент (имя программы)
    args.removeFirst();

    // Создаем менеджер файлов
    FileManager manager;

    for (const QString& filePath : args) {
        manager.addFile(filePath);
    }

    manager.printAllFilesInfo();


    manager.startMonitoring(100);

    logger.info("Мониторинг запущен. Для выхода нажмите Ctrl+C");

    //  цикл обработки событий
    return a.exec();
}

