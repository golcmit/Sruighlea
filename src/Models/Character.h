#pragma once

#include <QString>
#include <QDate>
#include <QList>
#include "./AcademicRecord.h"
#include "./OccupationRecord.h"
#include "./Relationship.h"
/**
 * @brief 学業記録の1項目
 */
    

/**
 * @brief キャラクターの全データを保持する構造体
 */
struct Character {
    // ID関連 (デフォルト値 -1)
    int id = -1;
    int schoolId = -1;
    int houseId = -1;
    int lineageId = -1;

    // 基本文字列情報
    QString fullName;
    QString nameComponents; // JSON
    QString sortName;
    QString bloodStatus;
    QString species = "Human";
    QString wand;           // JSON
    QString patronus;
    QString notes;

    // 日付情報
    QDate birthDate;
    QDate deathDate;
    
    // JOINで取得する関連名称
    QString schoolName;
    QString houseName;
    QString lineageName;

    // --- 経歴情報のリスト (WBS 2.4) ---
    QList<AcademicRecord> academics;
    QList<OccupationRecord> occupations;
    QList<Relationship> relationships;
    

    // --- ヘルパー関数 ---
    
    // データが有効か（DBから正しくロードされたか）のチェック
    bool isValid() const {
        return id != -1;
    }
};