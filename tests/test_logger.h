#pragma once

#include <QObject>

class TestLogger : public QObject
{
    Q_OBJECT

private slots:
    void testGetInstance_ReturnsSameInstance();
    void testLogToFile_CreatesFileAndWritesMessage();
};
