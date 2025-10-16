-- Sruighlea Neo Schema
-- Generated from schema_draft.md

-- =============================================================================
-- Master Data Tables
-- =============================================================================

-- 学校リスト
CREATE TABLE schools (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE
);

-- 寮リスト
CREATE TABLE houses (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE,
    school_id INTEGER NOT NULL,
    FOREIGN KEY (school_id) REFERENCES schools(id)
);

-- 家系リスト
CREATE TABLE lineages (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE,
    description TEXT
);

-- 科目リスト
CREATE TABLE subjects (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE
);

-- 役職リスト
CREATE TABLE roles (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE
);

-- グループリスト
CREATE TABLE groups (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE,
    description TEXT
);

-- 関係性の種類
CREATE TABLE relationship_types (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE,
    inverse_type_id INTEGER NOT NULL,
    is_bidirectional BOOLEAN NOT NULL DEFAULT 0,
    category TEXT,
    FOREIGN KEY (inverse_type_id) REFERENCES relationship_types(id)
);

-- =============================================================================
-- Core Table
-- =============================================================================

-- キャラクタープロフィール
CREATE TABLE characters (
    id INTEGER PRIMARY KEY,
    full_name TEXT NOT NULL,
    name_components TEXT, -- JSON
    sort_name TEXT,
    school_id INTEGER,
    house_id INTEGER,
    lineage_id INTEGER,
    blood_status TEXT,
    wand TEXT, -- JSON
    patronus TEXT,
    birth_date TEXT,
    death_date TEXT,
    species TEXT DEFAULT 'Human',
    notes TEXT,
    FOREIGN KEY (school_id) REFERENCES schools(id),
    FOREIGN KEY (house_id) REFERENCES houses(id),
    FOREIGN KEY (lineage_id) REFERENCES lineages(id)
);

-- =============================================================================
-- Relationship & History Tables
-- =============================================================================

-- 人間関係図
CREATE TABLE relationships (
    id INTEGER PRIMARY KEY,
    from_character_id INTEGER NOT NULL,
    to_character_id INTEGER NOT NULL,
    type_id INTEGER NOT NULL,
    start_date TEXT,
    end_date TEXT,
    notes TEXT,
    FOREIGN KEY (from_character_id) REFERENCES characters(id) ON DELETE CASCADE,
    FOREIGN KEY (to_character_id) REFERENCES characters(id) ON DELETE CASCADE,
    FOREIGN KEY (type_id) REFERENCES relationship_types(id)
);

-- 所属グループ
CREATE TABLE group_memberships (
    character_id INTEGER NOT NULL,
    group_id INTEGER NOT NULL,
    role TEXT,
    PRIMARY KEY (character_id, group_id),
    FOREIGN KEY (character_id) REFERENCES characters(id) ON DELETE CASCADE,
    FOREIGN KEY (group_id) REFERENCES groups(id) ON DELETE CASCADE
);

-- 学業記録
CREATE TABLE academic_records (
    id INTEGER PRIMARY KEY,
    character_id INTEGER NOT NULL,
    subject_id INTEGER NOT NULL,
    academic_year INTEGER NOT NULL,
    grade TEXT,
    is_owl BOOLEAN NOT NULL DEFAULT 0,
    is_newt BOOLEAN NOT NULL DEFAULT 0,
    notes TEXT,
    FOREIGN KEY (character_id) REFERENCES characters(id) ON DELETE CASCADE,
    FOREIGN KEY (subject_id) REFERENCES subjects(id)
);

-- 役職履歴
CREATE TABLE character_roles (
    id INTEGER PRIMARY KEY,
    character_id INTEGER NOT NULL,
    role_id INTEGER NOT NULL,
    appointed_year INTEGER NOT NULL,
    FOREIGN KEY (character_id) REFERENCES characters(id) ON DELETE CASCADE,
    FOREIGN KEY (role_id) REFERENCES roles(id)
);

-- 居住履歴
CREATE TABLE address_history (
    id INTEGER PRIMARY KEY,
    character_id INTEGER NOT NULL,
    address TEXT NOT NULL,
    start_date TEXT,
    end_date TEXT,
    FOREIGN KEY (character_id) REFERENCES characters(id) ON DELETE CASCADE
);

-- 職歴
CREATE TABLE occupation_history (
    id INTEGER PRIMARY KEY,
    character_id INTEGER NOT NULL,
    occupation TEXT NOT NULL,
    organization TEXT,
    start_date TEXT,
    end_date TEXT,
    FOREIGN KEY (character_id) REFERENCES characters(id) ON DELETE CASCADE
);