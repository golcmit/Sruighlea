// src/Relationship.h (たたき台)
#pragma once

#include <QString>
#include <QDate>

/*
 * @brief relationships (人間関係) テーブルの1レコードを保持するデータクラス
 */
class Relationship
{
public:
    Relationship()
        : id(-1), fromCharacterId(-1), toCharacterId(-1), typeId(-1)
    {}

    int id;               // id (PK)
    int fromCharacterId;  // from_character_id (FK)
    int toCharacterId;    // to_character_id (FK)
    int typeId;           // type_id (FK)
    
    QDate startDate;      // start_date
    QDate endDate;        // end_date
    QString notes;        // notes

    // データが有効か（DBからロードされたか）の簡易チェック
    bool isValid() const {
        return id != -1;
    }
};