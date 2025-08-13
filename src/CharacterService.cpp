#include "CharacterService.h"
#include <QSqlQuery>
#include <QVariant>

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
