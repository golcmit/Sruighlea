#include "CharacterService.h"
#include "Logger.h"
#include "SqlQueries.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlQueryModel>
#include <QSqlError>

CharacterService::CharacterService(QObject *parent) : QObject(parent)
{
}

// This is the minimal implementation to make the test compile.
// It doesn't have the logic yet, so the test will fail on assertion.
CharacterData CharacterService::getCharacterDetails(int characterId)
{
    CharacterData data;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(SqlQueries::GET_CHARACTER_DETAILS);
    query.bindValue(":id", characterId);

    if (query.exec() && query.next()) {
        data.id = query.value("id").toInt();
        data.firstName = query.value("first_name").toString();
        data.lastName = query.value("current_last_name").toString();
        data.birthDate = query.value("birth_date").toDate();
        data.house = query.value("house_name").toString();
        data.bloodStatus = query.value("blood_status").toString();
    }

    return data;
}

QSqlQueryModel* CharacterService::getCharacterListModel()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(SqlQueries::GET_CHARACTER_LIST);
    return model;
}

QSqlQueryModel* CharacterService::searchCharactersByName(const QString& name)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    
    QSqlQuery query;
    query.prepare(SqlQueries::SEARCH_CHARACTERS_BY_NAME);
    query.bindValue(":name", "%" + name + "%");
    query.exec();

    model->setQuery(query);
    
    return model;
}

bool CharacterService::addCharacter(const CharacterData& character)
{
    // 1. Find house_id from house name
    QSqlQuery houseQuery;
    houseQuery.prepare(SqlQueries::GET_HOUSE_ID_BY_NAME);
    houseQuery.bindValue(":name", character.house);
    if (!houseQuery.exec() || !houseQuery.next()) {
        Logger::instance().warning("Could not find house_id for house: " + character.house);
        return false; // House not found
    }
    int houseId = houseQuery.value(0).toInt();

    // 2. Prepare INSERT statement
    QSqlQuery insertQuery;
    insertQuery.prepare(SqlQueries::INSERT_CHARACTER);

    // 3. Bind values
    insertQuery.bindValue(":firstName", character.firstName);
    insertQuery.bindValue(":lastName", character.lastName);
    insertQuery.bindValue(":birthDate", character.birthDate);
    insertQuery.bindValue(":houseId", houseId);
    insertQuery.bindValue(":bloodStatus", character.bloodStatus);

    // 4. Execute
    if (!insertQuery.exec()) {
        Logger::instance().error("Failed to add character: " + insertQuery.lastError().text());
        return false;
    }

    return true;
}

bool CharacterService::updateCharacter(const CharacterData& character)
{
    // 1. Find house_id from house name
    QSqlQuery houseQuery;
    houseQuery.prepare(SqlQueries::GET_HOUSE_ID_BY_NAME);
    houseQuery.bindValue(":name", character.house);
    if (!houseQuery.exec() || !houseQuery.next()) {
        Logger::instance().warning("Could not find house_id for house: " + character.house);
        return false; // House not found
    }
    int houseId = houseQuery.value(0).toInt();

    // 2. Prepare UPDATE statement
    QSqlQuery updateQuery;
    updateQuery.prepare(SqlQueries::UPDATE_CHARACTER);

    // 3. Bind values
    updateQuery.bindValue(":firstName", character.firstName);
    updateQuery.bindValue(":lastName", character.lastName);
    updateQuery.bindValue(":birthDate", character.birthDate);
    updateQuery.bindValue(":houseId", houseId);
    updateQuery.bindValue(":bloodStatus", character.bloodStatus);
    updateQuery.bindValue(":id", character.id);

    // 4. Execute
    if (!updateQuery.exec()) {
        Logger::instance().error("Failed to update character: " + updateQuery.lastError().text());
        return false;
    }

    return true;
}
