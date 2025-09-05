#include "test_characterservice.h"
#include "../src/CharacterService.h"
#include "../src/DatabaseManager.h"

#include <QTest>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDate>
#include <QSqlQuery>

void TestCharacterService::testFetchCharacterDetails_ValidId_ReturnsData()
{
    // ARRANGE
    CharacterService service;

    // ACT
    // Assuming a character with ID 1 exists in the test database
    CharacterData details = service.getCharacterDetails(1);

    // ASSERT
    QVERIFY2(details.isValid(), "Test failed: Expected valid data for a valid ID, but got invalid data.");
    // ここも実際のデータベースのID=1のキャラクターデータに合わせて期待値を修正
    QCOMPARE(details.firstName, QString("Clover")); // 実際の値に合わせる
    QCOMPARE(details.lastName, QString("Gowin")); // ここを"Gowin"に修正
    QCOMPARE(details.house, QString("slytherin")); // ここを"slytherin"に修正
    QCOMPARE(details.bloodStatus, QString("pure")); // ここを"pure"に修正
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

void TestCharacterService::testFetchCharacterDetails_DefaultConnection_ValidId_ReturnsData()
{
    // ARRANGE
    // デフォルトのデータベース接続を確立 (main.cpp と同様)
    

    CharacterService service;

    // ACT
    // Assuming a character with ID 1 exists in the test database
    CharacterData details = service.getCharacterDetails(1);

    // ASSERT
    QVERIFY2(details.isValid(), "Test failed: Expected valid data for a valid ID using default connection, but got invalid data.");
    QCOMPARE(details.id, 1); // IDが正しいことを確認
    // ここで、データベース内のID=1のキャラクターのfirst_nameが"Harry"であることを仮定して検証
    // 実際のデータベースの内容に合わせて修正してね
    QCOMPARE(details.firstName, QString("Clover")); // 実際の値に合わせる
    QCOMPARE(details.lastName, QString("Gowin")); // ここを"Gowin"に修正
    QCOMPARE(details.house, QString("slytherin")); // ここを"slytherin"に修正
    QCOMPARE(details.bloodStatus, QString("pure")); // ここを"pure"に修正

    // データベース接続を閉じる
    
}

void TestCharacterService::testSearchCharactersByName_ExistingName_ReturnsMatchingModel()
{
    // ARRANGE
    CharacterService service;
    QString searchName = "Gowin"; // We know "Clover Gowin" exists from other tests.

    // ACT
    // This function doesn't exist yet, so this will fail to compile (RED)
    QSqlQueryModel* model = service.searchCharactersByName(searchName);

    // ASSERT
    QVERIFY(model != nullptr);
    QVERIFY2(model->rowCount() > 0, "Model should contain at least one character matching the search term.");

    bool foundClover = false;
    for (int i = 0; i < model->rowCount(); ++i) {
        if (model->record(i).value("first_name").toString() == "Clover" &&
            model->record(i).value("current_last_name").toString() == "Gowin") {
            foundClover = true;
            break;
        }
    }
    QVERIFY2(foundClover, "The search results should include 'Clover Gowin'.");

    delete model;
}

void TestCharacterService::testAddCharacter_ValidData_ReturnsSuccessAndAddsToDb()
{
    // ARRANGE
    CharacterService service;
    CharacterData newChar;
    newChar.firstName = "Kaede";
    newChar.lastName = "Kayano";
    newChar.house = "slytherin"; // Assuming house name, will need to resolve to house_id
    newChar.bloodStatus = "pure";
    newChar.birthDate = QDate(2001, 11, 9);

    // ACT
    bool result = service.addCharacter(newChar);

    // ASSERT
    QVERIFY2(result, "addCharacter should return true for valid data.");

    // Verify that the character was actually inserted
    QSqlQuery query;
    query.prepare("SELECT id FROM characters WHERE first_name = :firstName AND current_last_name = :lastName");
    query.bindValue(":firstName", newChar.firstName);
    query.bindValue(":lastName", newChar.lastName);
    QVERIFY2(query.exec(), "Query to find new character should execute successfully.");

    QVERIFY2(query.next(), "The new character should be found in the database after adding.");
    int newId = query.value(0).toInt();

    // CLEANUP
    query.prepare("DELETE FROM characters WHERE id = :id");
    query.bindValue(":id", newId);
    query.exec();
}

void TestCharacterService::testUpdateCharacter_ValidData_UpdatesInDb()
{
    // ARRANGE
    CharacterService service;
    // 1. Add a temporary character to ensure a known state
    CharacterData originalChar;
    originalChar.firstName = "Test";
    originalChar.lastName = "User";
    originalChar.house = "gryffindor";
    originalChar.bloodStatus = "half";
    originalChar.birthDate = QDate(2000, 1, 1);
    service.addCharacter(originalChar);

    // Find the ID of the newly added character
    QSqlQuery findQuery;
    findQuery.prepare("SELECT id FROM characters WHERE first_name = 'Test' AND current_last_name = 'User'");
    QVERIFY(findQuery.exec() && findQuery.next());
    int charId = findQuery.value(0).toInt();

    // 2. Fetch the character and modify it
    CharacterData charToUpdate = service.getCharacterDetails(charId);
    QVERIFY(charToUpdate.isValid());
    charToUpdate.house = "ravenclaw"; // Change the house

    // ACT
    bool result = service.updateCharacter(charToUpdate);

    // ASSERT
    QVERIFY2(result, "updateCharacter should return true for valid data.");

    // 4. Fetch again and verify the change
    CharacterData updatedChar = service.getCharacterDetails(charId);
    QVERIFY(updatedChar.isValid());
    QCOMPARE(updatedChar.house, QString("ravenclaw"));
    QCOMPARE(updatedChar.firstName, originalChar.firstName); // Ensure other data is unchanged

    // CLEANUP
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM characters WHERE id = :id");
    deleteQuery.bindValue(":id", charId);
    deleteQuery.exec();
}

