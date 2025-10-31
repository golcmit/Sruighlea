// Logger.h (修正後)
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
    ~Logger() override; // ★ QObjectを継承しているので override を推奨
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // ★ 削除: この行を削除します
    // static Logger* m_instance; 

    QFile* m_logFile = nullptr;
    QTextStream* m_logStream = nullptr;
};