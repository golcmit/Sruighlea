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

/**
 * @brief 指定されたIDのキャラクターの詳細情報を取得
 * @param characterId 取得するキャラクターのID
 * @return CharacterData キャラクターの詳細情報（取得失敗時は無効なデータ）
 * 
 * データベースから指定されたキャラクターの完全な情報を取得する
 * 名前、生年月日、所属寮、血統などの情報を含む
 */
CharacterData CharacterService::getCharacterDetails(int characterId)
{
    CharacterData data;
    
    // データベース接続を取得してクエリを準備
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(SqlQueries::GET_CHARACTER_DETAILS);
    query.bindValue(":id", characterId);
    
    // クエリを実行し、結果が存在する場合はデータを取得
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

/**
 * @brief 全キャラクターのリストを取得するためのモデルを作成
 * @return QSqlQueryModel* キャラクターリストのモデル（呼び出し側で管理）
 * 
 * データベースから全キャラクターのリストを取得し、
 * QTableViewなどで表示できるモデルとして返す
 */
QSqlQueryModel* CharacterService::getCharacterListModel()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(SqlQueries::GET_CHARACTER_LIST);
    return model;
}

/**
 * @brief 名前でキャラクターを検索
 * @param name 検索するキャラクター名（部分一致）
 * @return QSqlQueryModel* 検索結果のモデル（呼び出し側で管理）
 * 
 * 名前（ファーストネームまたはラストネーム）の部分一致で
 * キャラクターを検索し、結果をモデルとして返す
 */
QSqlQueryModel* CharacterService::searchCharactersByName(const QString& name)
{
    QSqlQueryModel* model = new QSqlQueryModel();
   
    // 名前検索用のクエリを準備
    QSqlQuery query;
    query.prepare(SqlQueries::SEARCH_CHARACTERS_BY_NAME);
    
    // ワイルドカードを使用して部分一致検索を実行
    query.bindValue(":name", "%" + name + "%");
    query.exec();
    
    // クエリ結果をモデルに設定
    model->setQuery(query);
   
    return model;
}

/**
 * @brief 新しいキャラクターをデータベースに追加
 * @param character 追加するキャラクターのデータ
 * @return bool 追加成功時true、失敗時false
 * 
 * 以下の手順でキャラクターを追加：
 * 1. 所属寮の名前からhouse_idを取得
 * 2. INSERT文を準備してデータをバインド
 * 3. クエリを実行
 */
bool CharacterService::addCharacter(const CharacterData& character)
{
    // 1. 寮の名前からhouse_idを取得
    QSqlQuery houseQuery;
    houseQuery.prepare(SqlQueries::GET_HOUSE_ID_BY_NAME);
    houseQuery.bindValue(":name", character.house);
    
    // 寮が見つからない場合はエラー
    if (!houseQuery.exec() || !houseQuery.next()) {
        Logger::instance().warning("Could not find house_id for house: " + character.house);
        return false; // 寮が見つからない
    }
    int houseId = houseQuery.value(0).toInt();
    
    // 2. INSERT文の準備
    QSqlQuery insertQuery;
    insertQuery.prepare(SqlQueries::INSERT_CHARACTER);
    
    // 3. 値をバインド
    insertQuery.bindValue(":firstName", character.firstName);
    insertQuery.bindValue(":lastName", character.lastName);
    insertQuery.bindValue(":birthDate", character.birthDate);
    insertQuery.bindValue(":houseId", houseId);
    insertQuery.bindValue(":bloodStatus", character.bloodStatus);
    
    // 4. クエリを実行
    if (!insertQuery.exec()) {
        Logger::instance().error("Failed to add character: " + insertQuery.lastError().text());
        return false;
    }
    
    return true;
}

/**
 * @brief 既存のキャラクター情報を更新
 * @param character 更新するキャラクターのデータ（IDを含む）
 * @return bool 更新成功時true、失敗時false
 * 
 * 以下の手順でキャラクターを更新：
 * 1. 所属寮の名前からhouse_idを取得
 * 2. UPDATE文を準備してデータをバインド
 * 3. クエリを実行
 */
bool CharacterService::updateCharacter(const CharacterData& character)
{
    // 1. 寮の名前からhouse_idを取得
    QSqlQuery houseQuery;
    houseQuery.prepare(SqlQueries::GET_HOUSE_ID_BY_NAME);
    houseQuery.bindValue(":name", character.house);
    
    // 寮が見つからない場合はエラー
    if (!houseQuery.exec() || !houseQuery.next()) {
        Logger::instance().warning("Could not find house_id for house: " + character.house);
        return false; // 寮が見つからない
    }
    int houseId = houseQuery.value(0).toInt();
    
    // 2. UPDATE文の準備
    QSqlQuery updateQuery;
    updateQuery.prepare(SqlQueries::UPDATE_CHARACTER);
    
    // 3. 値をバインド
    updateQuery.bindValue(":firstName", character.firstName);
    updateQuery.bindValue(":lastName", character.lastName);
    updateQuery.bindValue(":birthDate", character.birthDate);
    updateQuery.bindValue(":houseId", houseId);
    updateQuery.bindValue(":bloodStatus", character.bloodStatus);
    updateQuery.bindValue(":id", character.id); // 更新対象のIDを指定
    
    // 4. クエリを実行
    if (!updateQuery.exec()) {
        Logger::instance().error("Failed to update character: " + updateQuery.lastError().text());
        return false;
    }
    
    return true;
}