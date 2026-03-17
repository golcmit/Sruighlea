#include "Logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTextCodec>
#include <QDebug>



Logger& Logger::instance()
{

    // プログラム終了時に自動的にデストラクタが呼ばれます
    static Logger logger_instance;
    return logger_instance;
}

Logger::Logger(QObject *parent) : QObject(parent) {}

// デストラクタ
Logger::~Logger()
{
    // ★ デバッグ用に、デストラクタが呼ばれたことを確認します
    qDebug() << "Logger::~Logger(): Shutdown called.";
    shutdown();
}
//ロガー落とす
void Logger::shutdown()
{
    //バッファの内容をファイルに書き込む
    if (m_logStream) {
        m_logStream->flush();
    }
    //ファイルが開いていたら閉じる
    if (m_logFile && m_logFile->isOpen()) {
        m_logFile->close();
    }

    //メモリ解放
    delete m_logStream;
    m_logStream = nullptr;
    delete m_logFile;
    m_logFile = nullptr;
}

//ログファイル設定
void Logger::setLogFile(const QString& filePath)
{
    //一度ロガーを落とす。
    shutdown();
    //ファイルパスに応じてファイルオブジェクト作成
    m_logFile = new QFile(filePath);
    
    //ログを追記する
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