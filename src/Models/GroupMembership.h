// src/GroupMembership.h (たたき台)
#pragma once

#include <QString>

/*
 * @brief group_memberships (グループ所属) テーブルの1レコードを保持するデータクラス
 */
class GroupMembership
{
public:
    GroupMembership()
        : characterId(-1), groupId(-1)
    {}

    // 複合主キー
    int characterId;  // character_id (PK, FK)
    int groupId;      // group_id (PK, FK)
    
    QString role;     // role
    
    // データが有効か（DBからロードされたか）の簡易チェック
    bool isValid() const {
        return characterId != -1 && groupId != -1;
    }
};