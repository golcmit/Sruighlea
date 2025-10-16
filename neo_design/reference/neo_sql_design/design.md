。

### \#\# I. 設計思想とアーキチャ概要

本システムは、キャラクタ情報とその関係性を、最大限の安全性とデータ整合性を保ちながら管理することを目的とします。

* **データストア**: データストアは**PostgreSQL**に一元化します。関係性（グラフ構造）もRDB内で表現し、外部ファイルなど別のデータストアは持ちません。
* **アクセス制御**: データベースへのすべてのアクセスは、PythonやC++で実装された**専用のAPIラッパー**を介して行います。アプリケーションから直接SQLを実行することは**禁止**します。
* **書き込み制御**: データの追加・編集・削除といった書き込み操作は、システム全体で同時に一人のユーザーしか実行できない**排他制御モデル**を採用します。

-----

### \#\# II. データベーススキーマ (DDL)

データベースは以下の3つのテーブルで構成されます。

#### 1\. `characters` テーブル

キャラクターの基本情報と、名前の構造的要素を管理します。

```sql
CREATE TABLE characters (
    -- 主キー
    id BIGSERIAL PRIMARY KEY,

    -- 表示用の完全な名前 (最終的な信頼源)
    full_name TEXT NOT NULL,

    -- 名前の構造化された要素 (JSONB形式)
    name_components JSONB NOT NULL,
    
    -- 敬称・代名詞など、区別のための補足情報
    appendix_about_distinction TEXT,

    -- 楽観的ロック用のバージョン管理カラム (APIで利用)
    version INTEGER NOT NULL DEFAULT 1,

    -- ソート用の正規化された名前
    sort_name TEXT,

    -- タイムスタンプ
    created_at TIMESTAMPTZ NOT NULL DEFAULT now(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT now()
);

-- 主要な検索対象カラムにインデックスを設定
CREATE INDEX idx_characters_name_components ON characters USING GIN (name_components);
CREATE INDEX idx_characters_sort_name ON characters (sort_name);
```

#### 2\. `character_name_transcriptions` テーブル

名前の読み仮名や、別言語への転写情報を管理します。

```sql
CREATE TABLE character_name_transcriptions (
    -- 主キー
    id BIGSERIAL PRIMARY KEY,

    -- charactersテーブルへの外部キー (キャラクター削除時に自動で削除)
    character_id BIGINT NOT NULL REFERENCES characters(id) ON DELETE CASCADE,

    -- 転写体系 ('katakana', 'romaji', 'pinyin' など)
    "system" VARCHAR(50) NOT NULL,

    -- 転写された名前のコンポーネント (JSONB形式)
    components JSONB NOT NULL,

    -- タイムスタンプ
    created_at TIMESTAMPTZ NOT NULL DEFAULT now(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT now(),

    -- 同一キャラクターが同じ転写体系を複数持てないようにする制約
    UNIQUE (character_id, "system")
);

-- 外部キーと転写体系での検索を高速化
CREATE INDEX idx_character_name_transcriptions_character_id_system ON character_name_transcriptions (character_id, "system");
```

#### 3\. `information_flow_edges` テーブル

キャラクター間の関係性（有向グラフのエッジ）を管理します。

```sql
CREATE TABLE information_flow_edges (
    -- 情報の送信元（上流）
    source_character_id BIGINT NOT NULL REFERENCES characters(id) ON DELETE CASCADE,

    -- 情報の送信先（下流）
    target_character_id BIGINT NOT NULL REFERENCES characters(id) ON DELETE CASCADE,

    -- エッジの重複を防ぐための主キー
    PRIMARY KEY (source_character_id, target_character_id)
);

-- 逆引き検索を高速化
CREATE INDEX idx_edges_target_character_id ON information_flow_edges (target_character_id);
```

-----

### \#\# III. グラフデータベース（GDB）の概念的実装

本システムは、物理的に独立したグラフデータベースを持ちません。その代わりに、**リレーショナルデータベース内でグラフ構造を概念的に実装**します。これにより、トランザクションの完全な一貫性を保ちます。

* **ノード (Nodes)**: グラフの「ノード」は、`characters`テーブルの各行に相当します。
* **エッジ (Edges)**: グラフの「エッジ（有向の繋がり）」は、`information_flow_edges`テーブルの各行に相当します。このテーブルが、「誰から誰へ」という関係性を定義する**隣接リスト**として機能します。

グラフの探索（例：「特定キャラクター配下の全員をリストアップする」）は、SQLの\*\*再帰クエリ（Recursive CTE）\*\*を用いて行います。APIラッパーは、この再帰クエリを内部で実行することで、グラフ探索機能を提供します。

-----

### \#\# IV. 書き込み操作のワークフロー

全ての書き込み操作（`INSERT`, `UPDATE`, `DELETE`）は、以下の厳格な手順に従ってAPIラッパー内部で実行されます。これにより、排他性と原子性を完全に保証します。

1. **排他ロックの獲得**:
    操作の最初に、PostgreSQLの**アドバイザリロック** (`pg_try_advisory_lock`) を用いて、システム全体に対するグローバルな編集権を取得します。ロックに失敗した場合（他の誰かが操作中）、即座に処理を中断しエラーを返します。

2. **操作のバッファリング**:
    実行すべきSQLクエリ（例：`characters`への`INSERT`と`information_flow_edges`への`INSERT`）を、即時実行せずにアプリケーションのメモリ上の\*\*バッファ（リスト）\*\*に一時保管します。

3. **単一トランザクションでの実行**:
    データベースの**トランザクションを開始** (`BEGIN`) します。バッファに溜めた全てのSQLクエリを順次実行します。

      * 全てのクエリが成功した場合、トランザクションを**コミット** (`COMMIT`) して変更を確定します。
      * 一つでもクエリが失敗した場合、トランザクションを**ロールバック** (`ROLLBACK`) して、それまでに行った全ての変更を破棄します。

4. **排他ロックの解放**:
    トランザクションの成功・失敗に関わらず、最後に**必ずアドバイザリロックを解放** (`pg_advisory_unlock`) し、他のユーザが操作できるようにします。

-----

### \#\# V. 設計の主な利点

この設計により、以下の4つの重要な特性が確保されます。

* **データ整合性 (Integrity)**: トランザクションにより、`characters`テーブル（ノード）への変更と`information_flow_edges`テーブル（エッジ）への変更が常に一体として扱われるため、データの不整合は原理的に発生しません。
* **排他性 (Exclusivity)**: アドバイザリロックにより、重要なデータが同時に複数人によって変更されることがなく、意図しない上書きや競合を防ぎます。
* **安全性 (Safety)**: すべての操作がAPIラッパーを経由するため、不正なクエリの実行を防ぎ、ビジネスロジックを一元管理できます。
* **拡張性 (Extensibility)**: 将来的に新しい転写体系が必要になっても、`character_name_transcriptions`テーブルに行を追加するだけで対応可能です。
