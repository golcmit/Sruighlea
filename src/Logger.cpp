#include "Logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTextCodec>

Logger* Logger::m_instance = nullptr;

Logger& Logger::instance()
{
    if (!m_instance) {
        m_instance = new Logger();
    }
    return *m_instance;
}

Logger::Logger(QObject *parent) : QObject(parent) {}

Logger::~Logger()
{
    shutdown();
}

void Logger::shutdown()
{
    if (m_logStream) {
        m_logStream->flush();
    }
    if (m_logFile && m_logFile->isOpen()) {
        m_logFile->close();
    }
    delete m_logStream;
    m_logStream = nullptr;
    delete m_logFile;
    m_logFile = nullptr;
}

void Logger::setLogFile(const QString& filePath)
{
    shutdown();
    m_logFile = new QFile(filePath);
    if (m_logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_logStream = new QTextStream(m_logFile);
        m_logStream->setCodec(QTextCodec::codecForName("UTF-8"));
    }
}

void Logger::info(const QString& message)
{
    if (m_logStream) {
        (*m_logStream) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " [INFO]  " << message << "\n";
    }
}

void Logger::debug(const QString& message)
{
    if (m_logStream) {
        (*m_logStream) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " [DEBUG] " << message << "\n";
    }
}

void Logger::warning(const QString& message)
{
    if (m_logStream) {
        (*m_logStream) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " [WARN]  " << message << "\n";
    }
}

void Logger::error(const QString& message)
{
    if (m_logStream) {
        (*m_logStream) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " [ERROR] " << message << "\n";
    }
}

void Logger::critical(const QString& message)
{
    if (m_logStream) {
        (*m_logStream) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " [CRIT]  " << message << "\n";
    }
}