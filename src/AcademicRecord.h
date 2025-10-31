// src/AcademicRecord.h (たたき台)
#pragma once

#include <QString>

/*
 * @brief academic_records (学業記録) テーブルの1レコードを保持するデータクラス
 */
class AcademicRecord
{
public:
    AcademicRecord()
        : id(-1), characterId(-1), subjectId(-1),
          academicYear(0), isOwl(false), isNewt(false)
    {}

    int id;           // id (PK)
    int characterId;  // character_id (FK)
    int subjectId;    // subject_id (FK)
    int academicYear; // academic_year
    QString grade;    // grade
    bool isOwl;       // is_owl (BOOL)
    bool isNewt;      // is_newt (BOOL)
    QString notes;    // notes

    // データが有効か（DBからロードされたか）の簡易チェック
    bool isValid() const {
        return id != -1;
    }
};