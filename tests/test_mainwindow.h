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
    void testAddCharacterButton_ButtonExists();
    void testEditCharacterButton_ButtonExists();
    void testEditButton_InitiallyDisabled();

private:
    MainWindow *mainWindow;
};
