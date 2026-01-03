// src/GroupMembership.h (たたき台)
#pragma once

#include <QString>

/*
 * @brief group_memberships (グループ所属) テーブルの1レコードを保持するデータクラス
 */


struct GroupMembership
{
    int characterId=-1;
    int groupId=-1;
    bool isValid() const { return characterId != -1 &&groupId!=-1;}
};

