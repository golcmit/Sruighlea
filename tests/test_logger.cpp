#include "test_logger.h"
#include "../src/Logger.h" // This file doesn't exist yet
#include <QTest>
#include <QFile>
#include <QTextStream>

void TestLogger::testGetInstance_ReturnsSameInstance()
{
    Logger& logger1 = Logger::instance();
    Logger& logger2 = Logger::instance();
    QCOMPARE(&logger1, &logger2);
}

void TestLogger::testLogToFile_CreatesFileAndWritesMessage()
{
    // ARRANGE
    const QString logFilePath = "test_log.log";
    if (QFile::exists(logFilePath)) {
        QFile::remove(logFilePath);
    }

    Logger::instance().setLogFile(logFilePath);

    // ACT
    Logger::instance().info("This is a test message.");
    Logger::instance().shutdown(); // Ensure buffer is flushed

    // ASSERT
    QVERIFY(QFile::exists(logFilePath));

    QFile logFile(logFilePath);
    QVERIFY(logFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QString content = logFile.readAll();
    logFile.close();

    QVERIFY(content.contains("This is a test message."));

    // CLEANUP
    QFile::remove(logFilePath);
}
