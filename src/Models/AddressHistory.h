// src/AddressHistory.h (たたき台)
#pragma once

#include <QString>
#include <QDate>

/*
 * @brief address_history (住所履歴) テーブルの1レコードを保持するデータクラス
 */
class AddressHistory
{
public:
    AddressHistory()
        : id(-1), characterId(-1)
    {}

    int id;           // id (PK)
    int characterId;  // character_id (FK)
    QString address;  // address
    
    QDate startDate;  // start_date
    QDate endDate;    // end_date
    
    // データが有効か（DBからロードされたか）の簡易チェック
    bool isValid() const {
        return id != -1;
    }
};