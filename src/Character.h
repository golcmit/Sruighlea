// src/Character.h
#pragma once

#include <QString>
#include <QDate>

/*
 * @brief characters テーブルの1レコードを保持するデータクラス 
*/

class Character
{
public:
    //コンストラクタで初期化
    Character
        : id(-1),schoolId(-1),houseId(-1),linageId(-1)
    {}
    // --- メンバ変数 --- 

    int id; // id(PK)
    Qstring fullName; //full_name
    QString nameComponents; //name_components (jsonliteral)
    QString sortName; //sort_name

    // NullableなFKは -1 や 0 で初期化
    int schoolId;          // school_id (FK)
    int houseId;           // house_id (FK)
    int lineageId;         // lineage_id (FK)

    QString bloodStatus;   // blood_status
    QString wand;          // wand (JSONを文字列として)
    QString patronus;      // patronus

    // DBスキーマはTEXTだが、C++側ではQDateで扱う
    QDate birthDate;       // birth_date
    QDate deathDate;       // death_date (無効な日付
    
    QString species;       // species
    QString notes;         // notes

    // --- ヘルパー関数 ---
    
    // データが有効か（DBからロードされたか）の簡易チェック
    bool isValid() const {
        return id != -1;
    }
};