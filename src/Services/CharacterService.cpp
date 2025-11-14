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
    // (まだ実装しない)
    Logger::instance().debug(QString("getCharacterDetails called for id %1 (not implemented)").arg(characterId));
    return Character(); // 空のCharacterを返す
}
QList<OccupationRecord> CharacterService::getOccupationHistory(int characterId)
{
    
    Q_UNUSED(characterId); // 引数を使わない警告を抑制
    return QList<OccupationRecord>(); // 空のListを返す
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
    // 1. 新しいモデルインスタンスを作成
    QSqlQueryModel* model = new QSqlQueryModel();

    // 2. SqlQueries.h の GET_CHARACTER_LIST を使用
    model->setQuery(SqlQueries::GET_CHARACTER_LIST);

    // 3. エラーチェック (推奨)
    if (model->lastError().isValid()) {
        Logger::instance().error("Failed to get character list: " + model->lastError().text());
        delete model; // 失敗したので削除
        return nullptr;
    }

    // 4. モデルを返す
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

QList<AcademicRecord> CharacterService::getAcademicRecords(int characterId)
{
    Q_UNUSED(characterId);
    return QList<AcademicRecord>();
}

QList<Relationship> CharacterService::getRelationships(int characterId)
{
    Q_UNUSED(characterId);
    return QList<Relationship>();
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

bool CharacterService::updateCharacter(const Character& character)
{
    Q_UNUSED(character);
    return false;
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
