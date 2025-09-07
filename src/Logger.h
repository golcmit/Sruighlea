#pragma once

#include <QObject>
#include <QString>

class QFile;
class QTextStream;

class Logger : public QObject
{
    Q_OBJECT
public:
    static Logger& instance();

    void setLogFile(const QString& filePath);
    void debug(const QString& message);
    void info(const QString& message);
    void warning(const QString& message);
    void error(const QString& message);
    void critical(const QString& message);
    void shutdown(); // To close the file stream

private:
    Logger(QObject *parent = nullptr);
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger* m_instance;
    QFile* m_logFile = nullptr;
    QTextStream* m_logStream = nullptr;
};
