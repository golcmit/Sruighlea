// src/AddressHistory.h (たたき台)
#pragma once

#include <QString>
#include <QDate>

/*
 * @brief address_history (住所履歴) テーブルの1レコードを保持するデータクラス
 */
struct AddressHistory
{
    int id = -1;
    int characterId =-1;
    QString address;
    QDate startDate;  // start_date
    QDate endDate;    // end_date
    bool isValid() const { return id != -1; }
};

