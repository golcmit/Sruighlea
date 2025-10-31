// src/Services/CharacterService.h (修正後)
#pragma once

#include <QObject>
#include <QList> // QList を使うので include

// WBS 1.2 で作成したモデルクラスを include
// パスは CharacterService.h の場所から ../Models/ を想定
#include "../Models/Character.h"
#include "../Models/OccupationRecord.h"
#include "../Models/AcademicRecord.h"
#include "../Models/Relationship.h"
#include "../Models/GroupMembership.h"
#include "../Models/AddressHistory.h"

// QSqlQueryModel を使うので前方宣言（または include）
class QSqlQueryModel;

/* * ★削除★
 * 古い CharacterData struct は不要なので削除する
 * struct CharacterData { ... };
 */

class CharacterService : public QObject
{
    Q_OBJECT

public:
    explicit CharacterService(QObject *parent = nullptr);
    // デストラクタも定義しておくことを推奨
    ~CharacterService() override; 

    /*
     * WBS 1.3.1 (コメントアウト済み)
    CharacterData getCharacterDetails(int characterId);
    ...
    */

    // --- WBS 1.3.2: 新しい閲覧系メソッド ---

    /**
     * @brief メインの一覧表示用のモデルを取得する (id, full_name)
     */
    QSqlQueryModel* getCharacterListModel();
    
    /**
     * @brief [基本情報] タブ用の詳細データを取得する
     */
    Character getCharacterDetails(int characterId);

    /**
     * @brief [経歴] タブ用の職歴リストを取得する
     */
    QList<OccupationRecord> getOccupationHistory(int characterId);

    /**
     * @brief [学業] タブ用の成績リストを取得する
     */
    QList<AcademicRecord> getAcademicRecords(int characterId);

    /**
     * @brief [人間関係] タブ用の関係リストを取得する
     */
    QList<Relationship> getRelationships(int characterId);

    /**
     * @brief [所属] タブ用のグループ所属リストを取得する
     */
    QList<GroupMembership> getGroupMemberships(int characterId);

    /**
     * @brief [住所] タブ用の住所履歴リストを取得する
     */
    QList<AddressHistory> getAddressHistory(int characterId);

};