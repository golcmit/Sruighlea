#pragma once
#include <QString>
#include <QDate>

/**
 * @brief occupation_history テーブルの1レコード
 */
struct OccupationRecord {
    int id = -1;
    int characterId = -1;
    QString occupation;   // 役職
    QString organization; // 組織
    QDate startDate;
    QDate endDate;

    bool isValid() const { return id != -1; }
};