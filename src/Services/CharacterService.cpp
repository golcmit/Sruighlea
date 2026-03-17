#include "CharacterService.h"
#include "../Core/Logger.h"
#include "../Core/SqlQueries.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlQueryModel>
#include <QSqlError>

/**
 * @brief CharacterServiceのコンストラクタ
 * @param parent 親オブジェクト
 * 
 * キャラクターデータのビジネスロジックを提供するサービスクラス
 */
CharacterService::CharacterService(QObject *parent) : QObject(parent)
{
}
// デストラクタ
CharacterService::~CharacterService()
{
   
}
/**
 * @brief 指定されたIDのキャラクターの詳細情報を取得
 * @param characterId 取得するキャラクターのID
 * @return CharacterData キャラクターの詳細情報（取得失敗時は無効なデータ）
 * 
 * データベースから指定されたキャラクターの完全な情報を取得する
 * 名前、生年月日、所属寮、血統などの情報を含む
 */
Character CharacterService::getCharacterDetails(int characterId)
{
    Character c;
    QSqlQuery query;

    // 1. 基本情報の取得 (JOIN含む)
    query.prepare(
        "SELECT c.*, s.name AS school_name, h.name AS house_name, l.name AS lineage_name "
        "FROM characters c "
        "LEFT JOIN schools s ON c.school_id = s.id "
        "LEFT JOIN houses h ON c.house_id = h.id "
        "LEFT JOIN lineages l ON c.lineage_id = l.id "
        "WHERE c.id = :id"
    );
    //:id に値を埋める（SQLインジェクション対策）
    query.bindValue(":id", characterId);
    
    //SQL実行
    if (!query.exec() || !query.next()) {
        Logger::instance().error("Failed to fetch character basic info: " + query.lastError().text());
        return c;
    }

    // 値の取り出し
    c.id = query.value("id").toInt();
    c.fullName = query.value("full_name").toString();
    c.sortName = query.value("sort_name").toString();
    c.bloodStatus = query.value("blood_status").toString();
    c.patronus = query.value("patronus").toString();
    c.species = query.value("species").toString();
    c.wand = query.value("wand").toString();
    c.notes = query.value("notes").toString();
    c.birthDate = QDate::fromString(query.value("birth_date").toString(), Qt::ISODate);
    c.deathDate = QDate::fromString(query.value("death_date").toString(), Qt::ISODate);
    c.schoolName = query.value("school_name").toString();
    c.houseName = query.value("house_name").toString();
    c.lineageName = query.value("lineage_name").toString();

    // 2. 関連リストの取得 (各専門関数に委譲)
    c.academics = getAcademicRecords(characterId);
    c.occupations = getOccupationHistory(characterId);
    c.relationships = getRelationships(characterId);

    return c;
}

/**
 * @brief 学業記録リストの取得
 */
QList<AcademicRecord> CharacterService::getAcademicRecords(int characterId)
{
    QList<AcademicRecord> list;
    QSqlQuery query;
    query.prepare(
        "SELECT ar.*, s.name AS subject_name FROM academic_records ar "
        "JOIN subjects s ON ar.subject_id = s.id "
        "WHERE ar.character_id = :id ORDER BY ar.academic_year ASC"
    );
    query.bindValue(":id", characterId);

    if (query.exec()) {
        while (query.next()) {
            AcademicRecord rec;
            rec.id = query.value("id").toInt();
            rec.academicYear = query.value("academic_year").toInt();
            rec.subjectName = query.value("subject_name").toString();
            rec.grade = query.value("grade").toString();
            rec.isOwl = query.value("is_owl").toBool();
            rec.isNewt = query.value("is_newt").toBool();
            rec.notes = query.value("notes").toString();
            list.append(rec);
        }
    }
    return list;
}

/**
 * @brief 職歴リストの取得
 */
QList<OccupationRecord> CharacterService::getOccupationHistory(int characterId)
{
    QList<OccupationRecord> list;
    QSqlQuery query;
    query.prepare(
        "SELECT * FROM occupation_history WHERE character_id = :id "
        "ORDER BY start_date DESC"
    );
    query.bindValue(":id", characterId);

    if (query.exec()) {
        while (query.next()) {
            OccupationRecord rec;
            rec.id = query.value("id").toInt();
            rec.occupation = query.value("occupation").toString();
            rec.organization = query.value("organization").toString();
            rec.startDate = QDate::fromString(query.value("start_date").toString(), Qt::ISODate);
            rec.endDate = QDate::fromString(query.value("end_date").toString(), Qt::ISODate);
            list.append(rec);
        }
    }
    return list;
}

/**
 * @brief 人間関係リストの取得
 */
QList<Relationship> CharacterService::getRelationships(int characterId)
{
    QList<Relationship> list;
    QSqlQuery query;
    query.prepare(
        "SELECT r.*, c.full_name AS target_name, rt.name AS type_name "
        "FROM relationships r "
        "JOIN characters c ON r.to_character_id = c.id "
        "JOIN relationship_types rt ON r.relationship_type_id = rt.id "
        "WHERE r.from_character_id = :id"
    );
    query.bindValue(":id", characterId);

    if (query.exec()) {
        while (query.next()) {
            Relationship rel;
            rel.id = query.value("id").toInt();
            rel.fromCharacterId = query.value("from_character_id").toInt();
            rel.toCharacterId = query.value("to_character_id").toInt();
            rel.toCharacterName = query.value("target_name").toString();
            rel.relationshipTypeName = query.value("type_name").toString();
            rel.startDate = QDate::fromString(query.value("start_date").toString(), Qt::ISODate);
            rel.endDate = QDate::fromString(query.value("end_date").toString(), Qt::ISODate);
            rel.notes = query.value("notes").toString();
            list.append(rel);
        }
    }
    return list;
}





/**
 * @brief 全キャラクターのリストを取得するためのモデルを作成
 * @return QSqlQueryModel* キャラクターリストのモデル（呼び出し側で管理）
 * 
 * データベースから全キャラクターのリストを取得し、
 * QTableViewなどで表示できるモデルとして返す
 */
QSqlQueryModel* CharacterService::getCharacterListModel()
{
   QSqlDatabase db = QSqlDatabase::database(); // デフォルトの接続を取得
    if (!db.isOpen()) {
        Logger::instance().error("CharacterService: Database is NOT open!");
        return nullptr;
    }
    Logger::instance().debug("CharacterService: Database is open using connection: " + db.connectionName());

    // 2. クエリ実行
    QSqlQueryModel* model = new QSqlQueryModel();
    const QString queryStr = "SELECT id, full_name, sort_name FROM characters ORDER BY sort_name";
    
    model->setQuery(queryStr);

    // 3. 詳細なエラーチェック
    if (model->lastError().isValid()) {
        Logger::instance().error("SQL Error: " + model->lastError().text());
        Logger::instance().error("Executed Query: " + model->query().lastQuery());
    } else {
        int rows = model->rowCount();
        Logger::instance().info(QString("Query successful. Rows found: %1").arg(rows));
        
        // もし 0 件なら、テーブルの中身が見えていない
    }

    return model;
    

}


bool CharacterService::addCharacter(const Character& character)
{
    // (まだ実装しない)
    Q_UNUSED(character);
    return false;
}



bool CharacterService::addOccupationRecord(const OccupationRecord& record)
{
    Q_UNUSED(record);
    return false;
}





QList<GroupMembership> CharacterService::getGroupMemberships(int characterId)
{
    Q_UNUSED(characterId);
    return QList<GroupMembership>();
}

QList<AddressHistory> CharacterService::getAddressHistory(int characterId)
{
    Q_UNUSED(characterId);
    return QList<AddressHistory>();
}

/**
 * @brief キャラクター情報の更新
 */
bool CharacterService::updateCharacter(const Character& character)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE characters SET "
        "full_name = :full_name, sort_name = :sort_name, "
        "blood_status = :blood_status, patronus = :patronus, "
        "species = :species, notes = :notes, "
        "birth_date = :birth_date, death_date = :death_date "
        "WHERE id = :id"
    );

    query.bindValue(":full_name", character.fullName);
    query.bindValue(":sort_name", character.sortName);
    query.bindValue(":blood_status", character.bloodStatus);
    query.bindValue(":patronus", character.patronus);
    query.bindValue(":species", character.species);
    query.bindValue(":notes", character.notes);
    query.bindValue(":birth_date", character.birthDate.toString(Qt::ISODate));
    query.bindValue(":death_date", character.deathDate.toString(Qt::ISODate));
    query.bindValue(":id", character.id);

    if (!query.exec()) {
        Logger::instance().error("Failed to update character: " + query.lastError().text());
        return false;
    }
    return true;
}

bool CharacterService::deleteCharacter(int characterId)
{
    Q_UNUSED(characterId);
    return false;
}

bool CharacterService::updateOccupationRecord(const OccupationRecord& record)
{
    Q_UNUSED(record);
    return false;
}

bool CharacterService::deleteOccupationRecord(int recordId)
{
    Q_UNUSED(recordId);
    return false;
}

bool CharacterService::addRelationship(const Relationship& relationship)
{
    Q_UNUSED(relationship);
    return false;
}

bool CharacterService::updateRelationship(const Relationship& relationship)
{
    Q_UNUSED(relationship);
    return false;
}

bool CharacterService::deleteRelationship(int relationshipId)
{
    Q_UNUSED(relationshipId);
    return false;
}

bool CharacterService::addAcademicRecord(const AcademicRecord& record)
{
    Q_UNUSED(record);
    return false;
}

bool CharacterService::updateAcademicRecord(int recordId)
{
    Q_UNUSED(recordId);
    return false;
}

bool CharacterService::deleteAcademicRecord(int recordId)
{
    Q_UNUSED(recordId);
    return false;
}

bool CharacterService::addGroupMembership(const GroupMembership& membership)
{
    Q_UNUSED(membership);
    return false;
}

bool CharacterService::updateGroupMembership(int membershipId)
{
    Q_UNUSED(membershipId);
    return false;
}

bool CharacterService::deleteGroupMembership(int membershipId)
{
    Q_UNUSED(membershipId);
    return false;
}

bool CharacterService::addAddressHistory(const AddressHistory& record)
{
    Q_UNUSED(record);
    return false;
}

bool CharacterService::updateAddressHistory(int recordId)
{
    Q_UNUSED(recordId);
    return false;
}

bool CharacterService::deleteAddressHistory(int recordId)
{
    Q_UNUSED(recordId);
    return false;
}
