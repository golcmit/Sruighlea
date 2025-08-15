#pragma once

#include <QObject>

class MainWindow;

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSearchFunctionality_WhenTextEntered_FiltersList();

private:
    MainWindow *mainWindow;
};
