#!/bin/bash

DB_FILE="FanacalCharacters.db"

sqlite3 "$DB_FILE" <<'EOF'
PRAGMA foreign_keys = ON;
PRAGMA journal_mode = WAL; -- ロック少なく安全に
PRAGMA synchronous = NORMAL;

BEGIN TRANSACTION;

-- キャラクターテーブルからデータを取り出し、年齢計算を実行
WITH candidate_characters AS (
  SELECT
    c.id AS character_id,
    c.institution_id,
    c.birth_date,
    -- 誕生日が9月1日以降なら1年分追加（+12）、それ以外は11歳で入学
    CASE 
      WHEN strftime('%m-%d', c.birth_date) <= '08-31' THEN CAST(strftime('%Y', c.birth_date) AS INTEGER) + 11
      ELSE CAST(strftime('%Y', c.birth_date) AS INTEGER) + 12
    END AS enrollment_year
  FROM characters c
  WHERE c.birth_date IS NOT NULL
    AND c.institution_id IS NOT NULL
),

-- まだenrollmentに存在していないキャラクターのみ選定
new_enrollments AS (
  SELECT
    cc.character_id,
    cc.institution_id,
    cc.enrollment_year,
    cc.enrollment_year + 6 AS graduation_year
  FROM candidate_characters cc
  LEFT JOIN enrollment e ON cc.character_id = e.character_id
  WHERE e.character_id IS NULL
)

-- 新しいIDを作成するため、最大IDを取得して連番発番
INSERT INTO enrollment (id, character_id, institution_id, enrollment_year, graduation_year)
SELECT
  (SELECT IFNULL(MAX(id), 0) FROM enrollment) + ROW_NUMBER() OVER (ORDER BY character_id) AS new_id,
  ne.character_id,
  ne.institution_id,
  ne.enrollment_year,
  ne.graduation_year
FROM new_enrollments ne;

COMMIT;
EOF

echo "Enrollment table safely updated in $DB_FILE."
