#pragma once

#include <QObject>

class TestCharacterService : public QObject
{
    Q_OBJECT

private slots:
    void testFetchCharacterDetails_ValidId_ReturnsData();
    void testFetchCharacterDetails_InvalidId_ReturnsEmpty();
    void testFetchCharacterDetails_DefaultConnection_ValidId_ReturnsData(); // これを追加
    void testSearchCharactersByName_ExistingName_ReturnsMatchingModel();
    void testAddCharacter_ValidData_ReturnsSuccessAndAddsToDb();

};
