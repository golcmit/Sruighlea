#!/bin/bash

DB_FILE="FanacalCharacters.db"

# 引数: fromキャラクター名, toキャラクター名, 関係性名, 備考
FROM_CHARACTER_NAME="$1"
TO_CHARACTER_NAME="$2"
RELATIONSHIP_TYPE_NAME="$3"
NOTES="$4"

if [ -z "$FROM_CHARACTER_NAME" ] || [ -z "$TO_CHARACTER_NAME" ] || [ -z "$RELATIONSHIP_TYPE_NAME" ]; then
  echo "Usage: $0 <from_character_name> <to_character_name> <relationship_type_name> [notes]"
  exit 1
fi

sqlite3 "$DB_FILE" <<EOF
PRAGMA foreign_keys = ON;
BEGIN TRANSACTION;

-- 通常方向を登録
INSERT INTO relationships (id, from_character, to_character, relationship_type_id, notes)
VALUES (
  (SELECT IFNULL(MAX(id), 0) + 1 FROM relationships),
  (SELECT id FROM characters WHERE first_name || ' ' || current_last_name = '$FROM_CHARACTER_NAME' LIMIT 1),
  (SELECT id FROM characters WHERE first_name || ' ' || current_last_name = '$TO_CHARACTER_NAME' LIMIT 1),
  (SELECT id FROM relationship_types WHERE type_name = '$RELATIONSHIP_TYPE_NAME' LIMIT 1),
  '$NOTES'
);

-- 双方向必要なら逆向きも登録
INSERT INTO relationships (id, from_character, to_character, relationship_type_id, notes)
SELECT
  (SELECT IFNULL(MAX(id), 0) + 1 FROM relationships),
  (SELECT id FROM characters WHERE first_name || ' ' || current_last_name = '$TO_CHARACTER_NAME' LIMIT 1),
  (SELECT id FROM characters WHERE first_name || ' ' || current_last_name = '$FROM_CHARACTER_NAME' LIMIT 1),
  (SELECT id FROM relationship_types WHERE type_name = '$RELATIONSHIP_TYPE_NAME' LIMIT 1),
  '$NOTES'
FROM relationship_types
WHERE type_name = '$RELATIONSHIP_TYPE_NAME'
  AND is_bidirectional = 1;

COMMIT;
EOF

echo "Relationship(s) inserted into $DB_FILE."
