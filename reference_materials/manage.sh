#!/usr/bin/env bash

# ---------------------------------------------
# Unified DB Management Script for OriginalCharacters.db
# ---------------------------------------------
# Subcommands:
#   init                         Initialize DB (run once)
#   insert_characters <json>     Bulk-register characters from JSON
#   insert_relationships <json>  Bulk-register relationships from JSON
#   generate_enrollment          Auto-generate enrollment records
# ---------------------------------------------

DB_FILE="FanacalCharacters.db"

# Ensure sqlite3 and jq are available
for cmd in sqlite3 jq; do
  if ! command -v $cmd &>/dev/null; then
    echo "Error: '$cmd' is required. Please install it." >&2
    exit 1
  fi
done

# Initialize database and tables
init_db() {
  if [[ -f "$DB_FILE" ]]; then
    echo "Database '$DB_FILE' already exists. Skipping initialization."
    return
  fi
  sqlite3 "$DB_FILE" <<'EOF'
PRAGMA foreign_keys = ON;
PRAGMA journal_mode = WAL;
PRAGMA synchronous = NORMAL;

-- educational_institutions
CREATE TABLE IF NOT EXISTS educational_institutions (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL UNIQUE
);

-- houses
CREATE TABLE IF NOT EXISTS houses (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL UNIQUE,
  institution_id INTEGER,
  FOREIGN KEY (institution_id) REFERENCES educational_institutions(id),
  UNIQUE (id, institution_id)
);

-- lineages
CREATE TABLE IF NOT EXISTS lineages (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL UNIQUE,
  description TEXT
);

-- relationship_types
CREATE TABLE IF NOT EXISTS relationship_types (
  id INTEGER PRIMARY KEY,
  type_name TEXT NOT NULL UNIQUE,
  is_bidirectional BOOLEAN NOT NULL
);



-- characters
CREATE TABLE IF NOT EXISTS characters (
  id INTEGER PRIMARY KEY,
  first_name TEXT NOT NULL,
  middle_name TEXT,
  current_last_name TEXT NOT NULL,
  birth_last_name TEXT,
  institution_id INTEGER,
  house_id INTEGER,
  lineage_id INTEGER,
  birth_date DATE NOT NULL,
  death_date DATE,
  sex TEXT,
  gender TEXT,
  sexual_orientation TEXT,
  blood_status TEXT,
  notes TEXT,
  FOREIGN KEY(institution_id) REFERENCES educational_institutions(id),
  FOREIGN KEY(lineage_id) REFERENCES lineages(id),
  FOREIGN KEY(house_id, institution_id) REFERENCES houses(id, institution_id)
);

-- relationships
CREATE TABLE IF NOT EXISTS relationships (
  id INTEGER PRIMARY KEY,
  from_character INTEGER NOT NULL,
  to_character INTEGER NOT NULL,
  relationship_type_id INTEGER NOT NULL,
  notes TEXT,
  FOREIGN KEY(from_character) REFERENCES characters(id),
  FOREIGN KEY(to_character) REFERENCES characters(id),
  FOREIGN KEY(relationship_type_id) REFERENCES relationship_types(id)
);

-- enrollment
CREATE TABLE IF NOT EXISTS enrollment (
  id INTEGER PRIMARY KEY,
  character_id INTEGER NOT NULL,
  institution_id INTEGER NOT NULL,
  enrollment_year INTEGER NOT NULL,
  graduation_year INTEGER,
  notes TEXT,
  FOREIGN KEY(character_id) REFERENCES characters(id),
  FOREIGN KEY(institution_id) REFERENCES educational_institutions(id)
);

--初期値代入

--学校初期値
INSERT INTO educational_institutions (id, name) VALUES
    (0,'hogwarts'),
    (1,'beauxbâtons'),
    (2,'durmstrang'),
    (3,'ilvermorny'),
    (4,'koldovstoretz'),
    (5,'uagadou'),
    (6,'castelobruxo'),
    (7,'mahoutokoro');

--寮初期値
INSERT INTO houses (id, name) VALUES
    (0,'griffindor'),
    (1,'slytherin'),
    (2,'ravenclaw'),
    (3,'hufflepuff');

--血族初期値

INSERT INTO lineages (id, name,description) VALUES
    (0,'black','originally'),
    (1,'gaunt','originally'),
    (2,'lestrange','originally'),
    (3,'malfoy','originally'),
    (4,'carrow','originally'),
    (5,'greengrass','originally'),
    (6,'ollivander','originally'),
    (7,'parkinson','originally'),
    (8,'gowin','advocated_by_Seika');

INSERT OR IGNORE INTO relationship_types (id, type_name, is_bidirectional) VALUES
  (1, 'parent-child', 0),
  (2, 'friends', 1),
  (3, 'married', 1),
  (4, 'crush', 0),
  (5, 'sibling', 1);
  
EOF
  echo "Database initialized at '$DB_FILE'."
}

# Helper: fuzzy lookup via SOUNDEX then LIKE
get_id() {
  local table="$1" col="name" val="$2" id
  id=$(sqlite3 "$DB_FILE" <<SQL
SELECT id FROM $table
WHERE SOUNDEX($col)=SOUNDEX('$val') LIMIT 1;
SQL
)
  if [[ -z "$id" ]]; then
    id=$(sqlite3 "$DB_FILE" <<SQL
SELECT id FROM $table
WHERE $col LIKE '%'||'$val'||'%' LIMIT 1;
SQL
)
  fi
  if [[ -z "$id" ]]; then
    echo "Error: No match in '$table' for '$val'" >&2
    exit 1
  fi
  echo "$id"
}

# Subcommand: insert_characters <json_file>
insert_characters() {
  local jsonfile="$1"
  [[ -f "$jsonfile" ]] || { echo "JSON file not found: $jsonfile" >&2; exit 1; }
  jq -c '.characters[]' "$jsonfile" | while read -r obj; do
    FIRST_NAME=$(jq -r '.first_name' <<<"$obj")
    MIDDLE_NAME=$(jq -r '.middle_name // empty' <<<"$obj")
    CURRENT_LAST_NAME=$(jq -r '.current_last_name' <<<"$obj")
    BIRTH_LAST_NAME=$(jq -r '.birth_last_name // empty' <<<"$obj")
    INST_NAME=$(jq -r '.institution' <<<"$obj")
    HOUSE_NAME=$(jq -r '.house' <<<"$obj")
    LINEAGE_NAME=$(jq -r '.linage // empty' <<<"$obj")
    BIRTH_DATE=$(jq -r '.birth_date' <<<"$obj")
    DEATH_DATE=$(jq -r '.death_date // empty' <<<"$obj")
    SEX=$(jq -r '.sex // empty' <<<"$obj")
    GENDER=$(jq -r '.gender // empty' <<<"$obj")
    ORIENTATION=$(jq -r '.orientation // empty' <<<"$obj")
    BLOOD_STATUS=$(jq -r '.blood_status // empty' <<<"$obj")
    NOTES=$(jq -r '.notes // empty' <<<"$obj")

    IID=$(get_id educational_institutions "$INST_NAME")
    HID=$(get_id houses "$HOUSE_NAME")
    if [[ -n "$LINEAGE_NAME" ]]; then
      LGID=$(get_id lineages "$LINEAGE_NAME")
    else LGID=NULL; fi

    sqlite3 "$DB_FILE" <<SQL
PRAGMA foreign_keys=ON;
BEGIN;
INSERT INTO characters (id, first_name, middle_name, current_last_name, birth_last_name, institution_id, house_id, lineage_id, birth_date, death_date, sex, gender, sexual_orientation, blood_status, notes)
VALUES (
  (SELECT IFNULL(MAX(id),0)+1 FROM characters),
  '$FIRST_NAME',
  $( [[ -z $MIDDLE_NAME ]] && echo NULL || echo "'$MIDDLE_NAME'") ,
  '$CURRENT_LAST_NAME',
  $( [[ -z $BIRTH_LAST_NAME ]] && echo NULL || echo "'$BIRTH_LAST_NAME'") ,
  $IID,
  $HID,
  $LGID,
  '$BIRTH_DATE',
  $( [[ -z $DEATH_DATE ]] && echo NULL || echo "'$DEATH_DATE'") ,
  $( [[ -z $SEX ]] && echo NULL || echo "'$SEX'") ,
  $( [[ -z $GENDER ]] && echo NULL || echo "'$GENDER'") ,
  $( [[ -z $ORIENTATION ]] && echo NULL || echo "'$ORIENTATION'") ,
  $( [[ -z $BLOOD_STATUS ]] && echo NULL || echo "'$BLOOD_STATUS'") ,
  $( [[ -z $NOTES ]] && echo NULL || echo "'$NOTES'")
);
COMMIT;
SQL
    echo "Inserted character: $FIRST_NAME $CURRENT_LAST_NAME"
  done
}

# Subcommand: insert_relationships <json_file>
insert_relationships() {
  local jsonfile="$1"
  [[ -f "$jsonfile" ]] || { echo "JSON file not found: $jsonfile" >&2; exit 1; }
  jq -c '.relationships[]' "$jsonfile" | while read -r obj; do
    FROM_NAME=$(jq -r '.from' <<<"$obj")
    TO_NAME=$(jq -r '.to' <<<"$obj")
    TYPE_NAME=$(jq -r '.type' <<<"$obj")
    NOTES=$(jq -r '.notes // empty' <<<"$obj")

    FCID=$(sqlite3 "$DB_FILE" "SELECT id FROM characters WHERE first_name||' '||current_last_name LIKE '%$FROM_NAME%' LIMIT 1;")
    TCID=$(sqlite3 "$DB_FILE" "SELECT id FROM characters WHERE first_name||' '||current_last_name LIKE '%$TO_NAME%' LIMIT 1;")
    RTID=$(sqlite3 "$DB_FILE" "SELECT id,is_bidirectional FROM relationship_types WHERE type_name='\$TYPE_NAME' LIMIT 1;")
    # split RTID into two vars
    IFS='|' read -r REL_ID IS_BI <<<"\$(echo \$RTID | sed 's/,/|/')"

    sqlite3 "$DB_FILE" <<SQL
PRAGMA foreign_keys=ON;
BEGIN;
-- insert forward
INSERT INTO relationships (id, from_character, to_character, relationship_type_id, notes)
VALUES ((SELECT IFNULL(MAX(id),0)+1 FROM relationships), $FCID, $TCID, $REL_ID, \$( [[ -z $NOTES ]] && echo NULL || echo "'$NOTES'" ));
-- if bidirectional, insert reverse
\$( [[ "\$IS_BI" -eq 1 ]] && cat <<RSV
INSERT INTO relationships (id, from_character, to_character, relationship_type_id, notes)
VALUES ((SELECT IFNULL(MAX(id),0)+1 FROM relationships), $TCID, $FCID, $REL_ID, \$( [[ -z $NOTES ]] && echo NULL || echo "'$NOTES'" ));
RSV
)
COMMIT;
SQL
    echo "Inserted relationship: $FROM_NAME -> $TO_NAME ($TYPE_NAME)"
  done
}
# Subcommand: insert_lineages <json_file>
insert_lineages() {
  local jsonfile="$1"
  [[ -f "$jsonfile" ]] || { echo "JSON file not found: $jsonfile" >&2; exit 1; }

  jq -c '.lineages[]' "$jsonfile" | while read -r obj; do
    NAME=$(jq -r '.name' <<<"$obj")
    DESCRIPTION=$(jq -r '.description // empty' <<<"$obj")

    sqlite3 "$DB_FILE" <<SQL
PRAGMA foreign_keys=ON;
BEGIN;
INSERT INTO lineages (id, name, description)
VALUES (
  (SELECT IFNULL(MAX(id),0)+1 FROM lineages),
  '$NAME',
  $( [[ -z "$DESCRIPTION" ]] && echo NULL || echo "'$DESCRIPTION'" )
);
COMMIT;
SQL
    echo "Inserted lineage: $NAME"
  done
}

# Subcommand: generate_enrollment
generate_enrollment() {
  sqlite3 "$DB_FILE" <<'EOF'
PRAGMA foreign_keys = ON;
PRAGMA journal_mode = WAL;
PRAGMA synchronous = NORMAL;
BEGIN TRANSACTION;

WITH candidate_characters AS (
  SELECT id AS character_id, institution_id, birth_date,
    CASE WHEN strftime('%m-%d', birth_date) <= '08-31' THEN CAST(strftime('%Y', birth_date) AS INTEGER)+11
         ELSE CAST(strftime('%Y', birth_date) AS INTEGER)+12 END AS enrollment_year
  FROM characters WHERE birth_date IS NOT NULL AND institution_id IS NOT NULL
),
new_enrollments AS (
  SELECT cc.character_id, cc.institution_id, cc.enrollment_year, cc.enrollment_year+6 AS graduation_year
  FROM candidate_characters cc
  LEFT JOIN enrollment e ON cc.character_id=e.character_id
  WHERE e.character_id IS NULL
)
INSERT INTO enrollment (id, character_id, institution_id, enrollment_year, graduation_year)
SELECT (SELECT IFNULL(MAX(id),0)+ROW_NUMBER() OVER(ORDER BY character_id) FROM enrollment),
       ne.character_id, ne.institution_id, ne.enrollment_year, ne.graduation_year
FROM new_enrollments ne;

COMMIT;
EOF
  echo "Enrollment table updated."
}

case "$1" in
  init) init_db ;;
  insert_characters) insert_characters "$2" ;;
  insert_relationships) insert_relationships "$2" ;;
  generate_enrollment) generate_enrollment ;;
  insert_lineages) insert_lineages "$2" ;;
  *)
    echo "Usage: $0 {init|insert_characters <json>|insert_relationships <json>|generate_enrollment|insert_lineages <json>}";
    exit 1 ;;
esac

