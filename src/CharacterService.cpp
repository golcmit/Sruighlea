#include "CharacterService.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

CharacterService::CharacterService(QObject *parent) : QObject(parent)
{
}

// This is the minimal implementation to make the test compile.
// It doesn't have the logic yet, so the test will fail on assertion.
CharacterData CharacterService::getCharacterDetails(int characterId)
{
    CharacterData data;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT c.id, c.first_name, c.current_last_name, c.birth_date, h.name AS house_name, c.blood_status "
                  "FROM characters c LEFT JOIN houses h ON c.house_id = h.id "
                  "WHERE c.id = :id");
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
    QString query = "SELECT c.id, c.first_name, c.current_last_name, h.name AS house, c.blood_status FROM characters c LEFT JOIN houses h ON c.house_id = h.id ORDER BY c.id";
    model->setQuery(query);
    return model;
}

QSqlQueryModel* CharacterService::searchCharactersByName(const QString& name)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    
    QSqlQuery query;
    query.prepare("SELECT c.id, c.first_name, c.current_last_name, h.name AS house, c.blood_status "
                  "FROM characters c LEFT JOIN houses h ON c.house_id = h.id "
                  "WHERE c.first_name LIKE :name OR c.current_last_name LIKE :name "
                  "ORDER BY c.id");
    query.bindValue(":name", "%" + name + "%");
    query.exec();

    model->setQuery(query);
    
    return model;
}

bool CharacterService::addCharacter(const CharacterData& character)
{
    // 1. Find house_id from house name
    QSqlQuery houseQuery;
    houseQuery.prepare("SELECT id FROM houses WHERE name = :name");
    houseQuery.bindValue(":name", character.house);
    if (!houseQuery.exec() || !houseQuery.next()) {
        qWarning() << "Could not find house_id for house:" << character.house;
        return false; // House not found
    }
    int houseId = houseQuery.value(0).toInt();

    // 2. Prepare INSERT statement
    QSqlQuery insertQuery;
    insertQuery.prepare(
        "INSERT INTO characters (first_name, current_last_name, birth_date, house_id, blood_status) "
        "VALUES (:firstName, :lastName, :birthDate, :houseId, :bloodStatus)"
    );

    // 3. Bind values
    insertQuery.bindValue(":firstName", character.firstName);
    insertQuery.bindValue(":lastName", character.lastName);
    insertQuery.bindValue(":birthDate", character.birthDate);
    insertQuery.bindValue(":houseId", houseId);
    insertQuery.bindValue(":bloodStatus", character.bloodStatus);

    // 4. Execute
    if (!insertQuery.exec()) {
        qWarning() << "Failed to add character:" << insertQuery.lastError().text();
        return false;
    }

    return true;
}

bool CharacterService::updateCharacter(const CharacterData& character)
{
    // 1. Find house_id from house name
    QSqlQuery houseQuery;
    houseQuery.prepare("SELECT id FROM houses WHERE name = :name");
    houseQuery.bindValue(":name", character.house);
    if (!houseQuery.exec() || !houseQuery.next()) {
        qWarning() << "Could not find house_id for house:" << character.house;
        return false; // House not found
    }
    int houseId = houseQuery.value(0).toInt();

    // 2. Prepare UPDATE statement
    QSqlQuery updateQuery;
    updateQuery.prepare(
        "UPDATE characters SET "
        "first_name = :firstName, "
        "current_last_name = :lastName, "
        "birth_date = :birthDate, "
        "house_id = :houseId, "
        "blood_status = :bloodStatus "
        "WHERE id = :id"
    );

    // 3. Bind values
    updateQuery.bindValue(":firstName", character.firstName);
    updateQuery.bindValue(":lastName", character.lastName);
    updateQuery.bindValue(":birthDate", character.birthDate);
    updateQuery.bindValue(":houseId", houseId);
    updateQuery.bindValue(":bloodStatus", character.bloodStatus);
    updateQuery.bindValue(":id", character.id);

    // 4. Execute
    if (!updateQuery.exec()) {
        qWarning() << "Failed to update character:" << updateQuery.lastError().text();
        return false;
    }

    return true;
}
