#include "test_characterservice.h"
#include "../src/CharacterService.h" // This header does not exist yet

#include <QTest>
#include <QSqlDatabase>

void TestCharacterService::initTestCase()
{
    // This method is called before the first test case is executed.
    // We can use it to set up the database connection for all tests.
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "test_connection");
    db.setDatabaseName("../reference_materials/FanacalCharacters.db");
    if (!db.open()) {
        qFatal("Failed to open database for testing");
    }
}

void TestCharacterService::cleanupTestCase()
{
    // This method is called after the last test case has finished.
    // We can close the database connection here.
    QSqlDatabase::database("test_connection").close();
    QSqlDatabase::removeDatabase("test_connection");
}

void TestCharacterService::testFetchCharacterDetails_ValidId_ReturnsData()
{
    // ARRANGE
    CharacterService service;

    // ACT
    // Assuming a character with ID 1 exists in the test database
    CharacterData details = service.getCharacterDetails(1);

    // ASSERT
    QVERIFY2(details.isValid(), "Test failed: Expected valid data for a valid ID, but got invalid data.");
}

void TestCharacterService::testFetchCharacterDetails_InvalidId_ReturnsEmpty()
{
    // ARRANGE
    CharacterService service;

    // ACT
    CharacterData details = service.getCharacterDetails(-1); // Use an ID that is guaranteed to be invalid

    // ASSERT
    QVERIFY2(!details.isValid(), "Test failed: Expected invalid data for an invalid ID, but got valid data.");
}

