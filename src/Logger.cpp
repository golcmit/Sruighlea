#include "Logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTextCodec>
#include <QDebug>

// ★ 削除: この行を削除します
// Logger* Logger::m_instance = nullptr;

Logger& Logger::instance()
{
    // ★ 修正: この方式に変更します
    // プログラム終了時に自動的にデストラクタが呼ばれます
    static Logger logger_instance;
    return logger_instance;
}

Logger::Logger(QObject *parent) : QObject(parent) {}

// ★ 修正: override をつけます
Logger::~Logger()
{
    // ★ デバッグ用に、デストラクタが呼ばれたことを確認します
    qDebug() << "Logger::~Logger(): Shutdown called.";
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

        // ★成功したことを（一時的に）コンソールに出力
        qDebug() << "Logger::setLogFile: Successfully opened" << filePath;

    } else {
        // ★失敗した場合、コンソールにエラー理由を出力
        qDebug() << "Logger::setLogFile: FAILED to open" << filePath
                 << "Error:" << m_logFile->errorString();
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
        m_logStream->flush();
    }
}