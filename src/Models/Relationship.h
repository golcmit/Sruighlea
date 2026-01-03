#pragma once

#include <QString>
#include <QDate>

/**
 * @brief relationships テーブルの1レコードを保持する構造体
 */
struct Relationship
{
    int id = -1;
    int fromCharacterId = -1;
    int toCharacterId = -1;
    int typeId = -1;
    
    QDate startDate;
    QDate endDate;
    QString notes;

    // ★ 追加：JOIN で取得する「相手の名前」と「関係の種類名」
    QString toCharacterName;
    QString relationshipTypeName;

    bool isValid() const {
        return id != -1;
    }
};