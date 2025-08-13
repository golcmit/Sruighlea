#include "test_characterservice.h"
#include "../src/CharacterService.h" // This header does not exist yet

#include <QTest>
#include <QSqlDatabase>

void TestCharacterService::initTestCase()
{
    // デフォルトのデータベース接続を確立
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // デフォルト接続
    db.setDatabaseName("../reference_materials/FanacalCharacters.db");
    if (!db.open()) {
        qFatal("Failed to open database for testing");
    }
}

void TestCharacterService::cleanupTestCase()
{
    // デフォルトのデータベース接続を閉じる
    QSqlDatabase::database().close(); // デフォルト接続を閉じる
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection); // デフォルト接続を削除
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

