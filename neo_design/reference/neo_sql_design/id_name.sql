CREATE TABLE users (
    -- 主キー
    id BIGSERIAL PRIMARY KEY,

    -- 表示用の完全な名前 (絶対的なフォールバック先)
    full_name TEXT NOT NULL,

    -- 名前の構造化された要素を格納 (Source of Truth)
    -- 例: {"given": "Taro", "family": "Yamada"}
    -- 例: {"personal": "Aung San Suu Kyi"}
    -- 例: {"given": "Edwin", "particle": "van der", "family": "Sar"}
    name_components JSONB NOT NULL,

    -- 検索やソートを効率化するためのカラム (必要に応じて生成)
    -- 例: 'ヤマダ タロウ' や 'Smith, John'
    sort_name TEXT,
    appendix_about_distinction TEXT,


    -- 作成日時と更新日時
    created_at TIMESTAMPTZ NOT NULL DEFAULT now(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT now()
);

-- name_components と sort_name にはインデックスを張ることを推奨
CREATE INDEX idx_users_name_components ON users USING GIN (name_components);
CREATE INDEX idx_users_sort_name ON users (sort_name);


-- 転写情報
CREATE TABLE user_name_transcriptions (
    -- 主キー
    id BIGSERIAL PRIMARY KEY,

    -- usersテーブルへの外部キー
    user_id BIGINT NOT NULL REFERENCES users(id) ON DELETE CASCADE,

    -- 転写の体系を識別する文字列
    -- 例: 'katakana', 'romaji', 'pinyin', 'ipa'
    "system" VARCHAR(50) NOT NULL,

    -- 転写された名前のコンポーネント
    -- 例: {"family": "ヤマダ", "given": "タロウ"}
    components JSONB NOT NULL,

    -- 作成日時と更新日時
    created_at TIMESTAMPTZ NOT NULL DEFAULT now(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT now(),

    -- 同一ユーザーが同じ転写体系を複数持てないようにするユニーク制約
    UNIQUE (user_id, "system")
);

-- 外部キーと転写体系での検索を高速化するためのインデックス
CREATE INDEX idx_user_name_transcriptions_user_id_system ON user_name_transcriptions (user_id, "system");