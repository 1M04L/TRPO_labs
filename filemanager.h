#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include "filewatcher.h"

class FileManager : public QObject
{
    Q_OBJECT

public:
    explicit FileManager(QObject* parent = nullptr);
    ~FileManager();

    void addFile(const QString& filePath);
    void removeFile(const QString& filePath);
    void printAllFilesInfo() const;
    void startMonitoring(int intervalMs = 100);
    void stopMonitoring();

private slots:
    void checkAllFiles();

private:
    QVector<FileWatcher*> m_watchers;
    QTimer m_timer;
};

#endif // FILEMANAGER_H

