#pragma once
#include <QString>

/**
 * @brief academic_records テーブルの1レコード
 */
struct AcademicRecord {
    int id = -1;
    int characterId = -1;
    int subjectId = -1;
    int academicYear = 0;
    QString grade;
    bool isOwl = false;
    bool isNewt = false;
    QString notes;

    // ★ 追加：JOIN で取得する科目名
    QString subjectName;

    bool isValid() const { return id != -1; }
};