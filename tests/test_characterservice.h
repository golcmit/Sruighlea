#pragma once

#include <QObject>

class TestCharacterService : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(); // Called before the first test function is executed
    void cleanupTestCase(); // Called after the last test function was executed

    void testFetchCharacterDetails_ValidId_ReturnsData();
    void testFetchCharacterDetails_InvalidId_ReturnsEmpty();
    void testFetchCharacterDetails_DefaultConnection_ValidId_ReturnsData(); // これを追加

};
