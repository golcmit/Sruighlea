// src/Models/Character.h (修正後)
#pragma once

#include <QString>
#include <QDate>

/*
 * @brief characters テーブルの1レコードを保持するデータクラス
 */
class Character
{
public:
    // --- メンバ変数 (★先に宣言する) ---

    int id;                // id (PK)
    QString fullName;      // full_name (★タイポ修正: Qstring -> QString)
    QString nameComponents; // name_components (JSONを文字列として)
    QString sortName;      // sort_name

    int schoolId;          // school_id (FK)
    int houseId;           // house_id (FK)
    int lineageId;         // lineage_id (FK) (★エラーログでは 'linageId' になっていました。'lineageId' が正しいスペルです)

    QString bloodStatus;   // blood_status
    QString wand;          // wand (JSONを文字列として)
    QString patronus;      // patronus

    QDate birthDate;       // birth_date
    QDate deathDate;       // death_date
    
    QString species;       // species
    QString notes;         // notes

    // --- コンストラクタ (★メンバ変数の後に定義する) ---
    Character()
        : id(-1), schoolId(-1), houseId(-1), lineageId(-1)
    {}

    // --- ヘルパー関数 ---
    
    // データが有効か（DBからロードされたか）の簡易チェック
    bool isValid() const {
        return id != -1;
    }
};