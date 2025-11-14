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


    // --- WBS 1.3.3: 新しい編集系メソッド ---

    /**
     * @brief [基本情報] キャラクターを新規追加
     */
    bool addCharacter(const Character& character);

    /**
     * @brief [基本情報] キャラクター情報を更新
     */
    bool updateCharacter(const Character& character);
    
    // (キャラクターの削除も必要であれば)
    bool deleteCharacter(int characterId);

    /**
     * @brief [経歴] 職歴を追加
     */
    bool addOccupationRecord(const OccupationRecord& record);
    
    /**
     * @brief [経歴] 職歴を更新
     */
    bool updateOccupationRecord(const OccupationRecord& record);

    /**
     * @brief [経歴] 職歴を削除
     */
    bool deleteOccupationRecord(int recordId);

    /**
     * @brief [人間関係] 関係を追加
     * @note inverse_type_id に基づく双方向の登録ロジックを含む
     */
    bool addRelationship(const Relationship& relationship);
    
    /**
     * @brief [人間関係] 関係を更新
     */
    bool updateRelationship(const Relationship& relationship);

    /**
     * @brief [人間関係] 関係を削除
     */
    bool deleteRelationship(int relationshipId);

  

    /**
     * @brief [学業] 成績を追加
     */
    bool addAcademicRecord(const AcademicRecord& record);

    // 学業成績を更新
    bool updateAcademicRecord(int recordId)
    // 学業成績を削除
    bool deleteAcademicRecord(int recordId)

    /**
     * @brief [所属] グループ所属を追加
     */
    bool addGroupMembership(const GroupMembership& membership);
    // (update / delete も同様)
    bool updateGroupMembership(int membershipId)
    bool deleteGroupMembership(int membershipId)
    /**
     * @brief [住所] 住所履歴を追加
     */
    bool addAddressHistory(const AddressHistory& record);
    bool updateAddressHistory(int recordId);
    bool deleteAddressHistory(int recordId);
    // (update / delete も同様)

};