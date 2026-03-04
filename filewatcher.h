#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QFileInfo>
#include <QTimer>
#include <QString>

class FileWatcher : public QObject
{
    Q_OBJECT

public:
    explicit FileWatcher(const QString& filePath, QObject* parent = nullptr);
    ~FileWatcher();

    QString getFilePath() const;
    bool isFileExists() const;
    qint64 getFileSize() const;

public slots:
    void checkFile();

signals:
    void fileCreated(const QString& path, qint64 size);
    void fileDeleted(const QString& path);
    void fileChanged(const QString& path, qint64 newSize);
    void fileRestored(const QString& path, qint64 size);

private:
    QString m_filePath;
    QFileInfo m_fileInfo;
    bool m_lastExists;
    qint64 m_lastSize;

    bool updateFileInfo();
    void processChanges(bool exists, qint64 size);
};

#endif
