#pragma once
#include <QString>
#include <QDate>

/*
 * @brief occupation_history (職歴) テーブルの1レコードを保持するデータクラス
 */

class OccupationRecord
{
public:
    OccupationRecord()
    : id(-1), characterId(-1)
    {}

    int id;            // id (PK)
    int characterId;   // character_id (FK)
    QString occupation;  // occupation
    QString organization; // organization

    // 履歴データなので日付が重要
    QDate startDate;   // start_date
    QDate endDate;     // end_date (無効な日付
    
    // データが有効か（DBからロードされたか）の簡易チェック
    bool isValid() const {
        return id != -1;
    }
}