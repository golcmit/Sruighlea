# 新データベース設計案（たたき台）

##  guiding principles

1.  **誰でも使いやすい**: IT技術者でなくても直感的に操作できる。
2.  **ハリポタ特化かつ汎用的**: ハリー・ポッターの二次創作に最適化しつつ、他の世界観にも転用可能な柔軟性を持つ。
3.  **複雑な関係性を扱える**: キャラクター間の入り組んだ人間関係を豊かに表現できる。
4.  **シンプルで保守しやすい**: 個人開発であることを踏まえ、複雑すぎず、理解しやすく、メンテナンスが容易な構造を維持する。

---

## 1. `characters` テーブル（プロフィール帳）

キャラクター個人の基本的な情報を管理する中心的なテーブル。

| カラム名 | 型 | 説明 |
| :--- | :--- | :--- |
| `id` | `INTEGER` | 主キー。キャラクターを一位に識別するID。 |
| `full_name` | `TEXT` | 表示用のフルネーム。 |
| `name_components` | `TEXT` (JSON) | `{"given": "名", "family": "姓", "nickname": "愛称"}` のように、名前の構成要素をJSONで格納。検索の柔軟性を高める。 |
| `sort_name` | `TEXT` | 「姓, 名」の形式など、ソート（並べ替え）に使うための名前。 |
| `school_id` | `INTEGER` | 所属する学校のID。（`schools`テーブルへの外部キー） |
| `house_id` | `INTEGER` | 所属する寮のID。（`houses`テーブルへの外部キー） |
| `lineage_id` | `INTEGER` | 所属する家系のID。（`lineages`テーブルへの外部キー） |
| `blood_status` | `TEXT` | 血の純度（例：「純血」「半純血」「マグル生まれ」）。 |
| `wand` | `TEXT` (JSON) | 杖の情報をJSONで格納（例：`{"wood": "柊", "core": "不死鳥の羽根"}`）。 |
| `patronus` | `TEXT` | 守護霊。 |
| `birth_date` | `TEXT` | 誕生日（ISO 8601形式: `YYYY-MM-DD`）。 |
| `death_date` | `TEXT` | 没日（ISO 8601形式: `YYYY-MM-DD`）。 |
| `species` | `TEXT` | 種族（例：「人間」「屋敷しもべ妖精」「ゴブリン」）。 |
| `notes` | `TEXT` | 自由記述の備考欄。 |

---

## 2. `relationships` テーブル（人間関係図）

キャラクター間の関係性を定義する。このシステムの核となる部分。関係性の時間的な変化やニュアンスも記録できるようにする。

| カラム名 | 型 | 説明 |
| :--- | :--- | :--- |
| `id` | `INTEGER` | 主キー。 |
| `from_character_id` | `INTEGER` | 関係元のキャラクターID。 |
| `to_character_id` | `INTEGER` | 関係先のキャラクターID。 |
| `type_id` | `INTEGER` | 関係性の種類を示すID。（`relationship_types`テーブルへの外部キー） |
| `start_date` | `TEXT` | この関係が始まった日（ISO 8601形式）。 |
| `end_date` | `TEXT` | この関係が終わった日（ISO 8601形式）。NULLの場合は現在も継続中。 |
| `notes` | `TEXT` | 関係性に関する自由記述のメモ（例：「一方的にライバル視している」など）。 |


### 関連テーブル: `relationship_types`

関係性の「種類」を定義する。反対の関係性を指定することで、データ入力を補助し、一貫性を保つ。

| カラム名 | 型 | 説明 |
| :--- | :--- | :--- |
| `id` | `INTEGER` | 主キー。 |
| `name` | `TEXT` | 関係性の名前（例：「親」「子」「友達」）。 |
| `inverse_type_id` | `INTEGER` | 反対の関係性のID。「親」の反対は「子」。自分自身を指す場合は双方向（例：「友達」）。 |
| `is_bidirectional` | `BOOLEAN` | `inverse_type_id`が自分自身を指す場合に `true` となる、実質的な冗長カラム。UIでの扱いを簡単にするために持つ。 |
| `category` | `TEXT` | 関係性のカテゴリ（例：「家族」「恋愛」「対立」）。分類に使う。 |

---

## 3. `groups` / `group_memberships` テーブル（所属グループ）

キャラクターが所属する組織や団体を管理する。

### `groups` テーブル

| カラム名 | 型 | 説明 |
| :--- | :--- | :--- |
| `id` | `INTEGER` | 主キー。 |
| `name` | `TEXT` | グループ名（例：「不死鳥の騎士団」「ダンブルドア軍団」）。 |
| `description` | `TEXT` | グループの説明。 |

### `group_memberships` テーブル (中間テーブル)

| カラム名 | 型 | 説明 |
| :--- | :--- | :--- |
| `character_id` | `INTEGER` | キャラクターID。 |
| `group_id` | `INTEGER` | グループID。 |
| `role` | `TEXT` | グループ内での役割（例：「リーダー」「メンバー」）。 |

---

## 4. `academic_records` テーブル（学業記録）

生徒の年度ごとの成績や、O.W.L.・N.E.W.T.といった特別な試験結果を記録する。

| カラム名 | 型 | 説明 |
| :--- | :--- | :--- |
| `id` | `INTEGER` | 主キー。 |
| `character_id` | `INTEGER` | 生徒のID。 |
| `subject_id` | `INTEGER` | 科目のID。（`subjects`テーブルへの外部キー） |
| `academic_year` | `INTEGER` | 学年（例：1, 2, 3, 5...）。 |
| `grade` | `TEXT` | その学年の最終成績（例：「O」「E」「A」...）。 |
| `is_owl` | `BOOLEAN` | この成績がO.W.L.試験の結果なら `true`。 |
| `is_newt` | `BOOLEAN` | この成績がN.E.W.T.試験の結果なら `true`。 |
| `notes` | `TEXT` | 備考欄（例：「実技は特に優秀」とか）。 |

---

## 5. `character_roles` テーブル（役職）

生徒が務めた役職（監督生など）を記録する。

| カラム名 | 型 | 説明 |
| :--- | :--- | :--- |
| `id` | `INTEGER` | 主キー。 |
| `character_id` | `INTEGER` | 生徒のID。 |
| `role_id` | `INTEGER` | 役職のID。（`roles`テーブルへの外部キー） |
| `appointed_year` | `INTEGER` | 役職に任命された学年。 |

---

## 6. `address_history` テーブル（居住履歴）

キャラクターの居住地の変遷を記録する。

| カラム名 | 型 | 説明 |
| :--- | :--- | :--- |
| `id` | `INTEGER` | 主キー。 |
| `character_id` | `INTEGER` | キャラクターのID。 |
| `address` | `TEXT` | 住所。 |
| `start_date` | `TEXT` | 住み始めた日（ISO 8601形式）。 |
| `end_date` | `TEXT` | 引っ越した日（ISO 8601形式）。NULLの場合は現在も居住。 |

---

## 7. `occupation_history` テーブル（職歴）

キャラクターの職業の変遷を記録する。

| カラム名 | 型 | 説明 |
| :--- | :--- | :--- |
| `id` | `INTEGER` | 主キー。 |
| `character_id` | `INTEGER` | キャラクターのID。 |
| `occupation` | `TEXT` | 職業名（例：「魔法省大臣」「闇の魔術に対する防衛術教授」）。 |
| `organization` | `TEXT` | 所属組織（例：「魔法省」「ホグワーツ」）。 |
| `start_date` | `TEXT` | 就職日（ISO 8601形式）。 |
| `end_date` | `TEXT` | 退職日（ISO 8601形式）。NULLの場合は現在も在職。 |

---

## 8. その他（マスタデータ用テーブル）

正規化のための、選択肢を管理する小さなテーブル群。

*   **`schools`**: 学校リスト (`id`, `name`)
*   **`houses`**: 寮リスト (`id`, `name`, `school_id`)
*   **`lineages`**: 家系リスト (`id`, `name`, `description`)
*   **`subjects`**: 科目リスト (`id`, `name`)
*   **`roles`**: 役職リスト (`id`, `name`)
