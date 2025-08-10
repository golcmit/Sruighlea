#!/bin/bash

DB_FILE="FanacalCharacters.db"

# 引数の定義
FIRST_NAME="$1"
MIDDLE_NAME="$2"
CURRENT_LAST_NAME="$3"
BIRTH_LAST_NAME="$4"
INSTITUTION_NAME="$5"
HOUSE_NAME="$6"
LINEAGE_NAME="$7"
BIRTH_DATE="$8"
DEATH_DATE="$9"
SEX="${10}"
GENDER="${11}"
SEXUAL_ORIENTATION="${12}"
BLOOD_STATUS="${13}"
NOTES="${14}"

# 必須チェック
if [ -z "$FIRST_NAME" ] || [ -z "$CURRENT_LAST_NAME" ] || [ -z "$INSTITUTION_NAME" ] || [ -z "$HOUSE_NAME" ] || [ -z "$BIRTH_DATE" ]; then
  echo "Usage: $0 <first_name> <middle_name> <current_last_name> <birth_last_name> <institution_name> <house_name> <lineage_name> <birth_date> <death_date> <sex> <gender> <sexual_orientation> <blood_status> <notes>"
  exit 1
fi

# idを取得する関数
get_id() {
  TABLE="$1"
  NAME="$2"
  COLUMN="name"

  ID=$(sqlite3 "$DB_FILE" <<EOF
SELECT id FROM $TABLE
WHERE SOUNDEX($COLUMN) = SOUNDEX('$NAME')
LIMIT 1;
EOF
)

  if [ -z "$ID" ]; then
    # 見つからなかったらLIKE検索
    ID=$(sqlite3 "$DB_FILE" <<EOF
SELECT id FROM $TABLE
WHERE $COLUMN LIKE '%' || '$NAME' || '%'
LIMIT 1;
EOF
)
  fi

  if [ -z "$ID" ]; then
    echo "Error: Could not find matching entry in $TABLE for '$NAME'"
    exit 1
  fi

  echo "$ID"
}

INSTITUTION_ID=$(get_id "educational_institutions" "$INSTITUTION_NAME")
HOUSE_ID=$(get_id "houses" "$HOUSE_NAME")

# 血族は空欄許可
if [ -n "$LINEAGE_NAME" ]; then
  LINEAGE_ID=$(get_id "lineages" "$LINEAGE_NAME")
else
  LINEAGE_ID="NULL"
fi

# SQL実行
sqlite3 "$DB_FILE" <<EOF
PRAGMA foreign_keys = ON;
BEGIN TRANSACTION;

INSERT INTO characters (
    id,
    first_name,
    middle_name,
    current_last_name,
    birth_last_name,
    institution_id,
    house_id,
    lineage_id,
    birth_date,
    death_date,
    sex,
    gender,
    sexual_orientation,
    blood_status,
    notes
)
VALUES (
    (SELECT IFNULL(MAX(id), 0) + 1 FROM characters),
    '$FIRST_NAME',
    CASE WHEN '$MIDDLE_NAME' = '' THEN NULL ELSE '$MIDDLE_NAME' END,
    '$CURRENT_LAST_NAME',
    CASE WHEN '$BIRTH_LAST_NAME' = '' THEN NULL ELSE '$BIRTH_LAST_NAME' END,
    $INSTITUTION_ID,
    $HOUSE_ID,
    $LINEAGE_ID,
    '$BIRTH_DATE',
    CASE WHEN '$DEATH_DATE' = '' THEN NULL ELSE '$DEATH_DATE' END,
    CASE WHEN '$SEX' = '' THEN NULL ELSE '$SEX' END,
    CASE WHEN '$GENDER' = '' THEN NULL ELSE '$GENDER' END,
    CASE WHEN '$SEXUAL_ORIENTATION' = '' THEN NULL ELSE '$SEXUAL_ORIENTATION' END,
    CASE WHEN '$BLOOD_STATUS' = '' THEN NULL ELSE '$BLOOD_STATUS' END,
    CASE WHEN '$NOTES' = '' THEN NULL ELSE '$NOTES' END
);

COMMIT;
EOF

echo "Character inserted successfully into $DB_FILE."
