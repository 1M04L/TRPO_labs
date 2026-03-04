#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QMutex>
#include <iostream>

class Logger {
public:
    static Logger& instance() {
        static Logger instance;
        return instance;
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;


    void info(const QString& message) {
        QMutexLocker locker(&m_mutex);
        log("INFO", message);
    }

    void event(const QString& message) {
        QMutexLocker locker(&m_mutex);
        log("EVENT", message);
    }

    void error(const QString& message) {
        QMutexLocker locker(&m_mutex);
        log("ERROR", message);
    }

    // можно вкл/выкл запись в файл
    void setLogToFile(bool enabled, const QString& filename = "log.txt") {
        QMutexLocker locker(&m_mutex);
        m_logToFile = enabled;
        if (enabled) {
            m_logFile.setFileName(filename);
            m_logFile.open(QIODevice::Append | QIODevice::Text);
        } else {
            if (m_logFile.isOpen()) {
                m_logFile.close();
            }
        }
    }

private:
    Logger() : m_logToFile(false) {}

    ~Logger() {
        if (m_logFile.isOpen()) {
            m_logFile.close();
        }
    }

    // Внутренний метод для форматирования и вывода лога
    void log(const QString& level, const QString& message) {
        QString formattedMessage = QString("[%1] [%2] %3")
                                       .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                                       .arg(level)
                                       .arg(message);

        // в консоль
        QTextStream out(stdout);
        out << formattedMessage << "\n";
        out.flush();

        //запись в файл
        if (m_logToFile && m_logFile.isOpen()) {
            QTextStream fileStream(&m_logFile);
            fileStream << formattedMessage << "\n";
            fileStream.flush();
        }
    }

    QFile m_logFile;
    bool m_logToFile;
    QMutex m_mutex;
};

#endif // LOGGER_H
