```mermaid
erDiagram
    characters {
        INTEGER id PK "キャラクターID"
        TEXT first_name "名"
        TEXT middle_name "ミドルネーム"
        TEXT current_last_name "現在の姓"
        TEXT birth_last_name "出生時の姓"
        INTEGER institution_id FK "学校ID"
        INTEGER house_id FK "寮ID"
        INTEGER lineage_id FK "血族ID"
        DATE birth_date "誕生日"
        DATE death_date "没日"
        TEXT sex "生物学的性"
        TEXT gender "ジェンダー"
        TEXT sexual_orientation "性的指向"
        TEXT blood_status "血の純度"
        TEXT notes "備考"
    }

    educational_institutions {
        INTEGER id PK "学校ID"
        TEXT name UNIQUE "学校名"
    }

    houses {
        INTEGER id PK "寮ID"
        TEXT name UNIQUE "寮名"
        INTEGER institution_id FK "学校ID"
    }

    lineages {
        INTEGER id PK "血族ID"
        TEXT name UNIQUE "血族名"
        TEXT description "説明"
    }

    enrollment {
        INTEGER id PK "在籍情報ID"
        INTEGER character_id FK "キャラクターID"
        INTEGER institution_id FK "学校ID"
        INTEGER enrollment_year "入学年"
        INTEGER graduation_year "卒業年"
        TEXT notes "備考"
    }

    relationships {
        INTEGER id PK "関係ID"
        INTEGER from_character FK "関係元キャラクターID"
        INTEGER to_character FK "関係先キャラクターID"
        INTEGER relationship_type_id FK "関係種別ID"
        TEXT notes "備考"
    }

    relationship_types {
        INTEGER id PK "関係種別ID"
        TEXT type_name "関係名"
        BOOLEAN is_bidirectional "双方向フラグ"
    }

    educational_institutions ||--o{ houses : "に所属する"
    educational_institutions ||--o{ characters : "に所属する"
    educational_institutions ||--o{ enrollment : "の在籍情報"
    houses ||--o{ characters : "に所属する"
    lineages ||--o{ characters : "に所属する"
    characters ||--o{ enrollment : "の在籍情報"
    characters ||--o{ relationships : "の関係元"
    characters ||--o{ relationships : "の関係先"
    relationship_types ||--o{ relationships : "の種類"

```