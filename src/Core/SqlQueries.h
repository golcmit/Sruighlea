#pragma once

#include <QString>

namespace SqlQueries {

    //キャラクタID,ファーストネーム,ラストネーム,誕生日,寮の名前,血統.
    // これらはキャラクタ表とそこにidの一致での左優先結合による寮表
    //ただしキャラクタIDは指定されたもの
constexpr const char* GET_CHARACTER_DETAILS =
    "SELECT c.id, c.first_name, c.current_last_name, c.birth_date, h.name AS house_name, c.blood_status "
    "FROM characters c LEFT JOIN houses h ON c.house_id = h.id "
    "WHERE c.id = :id";

    //キャラクタ表より、id,フルネーム,ソート用氏名.ソート用氏名順で並べる。
constexpr const char* GET_CHARACTER_LIST =
    "SELECT id, full_name, sort_name FROM characters ORDER BY sort_name";
    
    //キャラクタID,ファーストネーム,ラストネーム,誕生日,寮の名前,血統.
    //これらはキャラクタ表とそこにidの一致での左優先結合による寮表
    //ただし,名 または 姓 が指定されたパターンに一致するキャラを検索
    //キャラクタIDによって並び替え
constexpr const char* SEARCH_CHARACTERS_BY_NAME =
    "SELECT c.id, c.first_name, c.current_last_name, h.name AS house, c.blood_status "
    "FROM characters c LEFT JOIN houses h ON c.house_id = h.id "
    "WHERE c.first_name LIKE :name OR c.current_last_name LIKE :name "
    "ORDER BY c.id";
    
    //寮ID,寮表から,指定された名前から
constexpr const char* GET_HOUSE_ID_BY_NAME =
    "SELECT id FROM houses WHERE name = :name";

    //ファーストネーム,今のラストネーム,誕生日,寮id,血統,これらによってキャラクタ表に挿入
    //呼び出し側での変数名指定は(firstName, lastName, birthDate, houseId, bloodStatus)
constexpr const char* INSERT_CHARACTER =
    "INSERT INTO characters (first_name, current_last_name, birth_date, house_id, blood_status) "
    "VALUES (:firstName, :lastName, :birthDate, :houseId, :bloodStatus)";

constexpr const char* UPDATE_CHARACTER =
    "UPDATE characters SET "
    "first_name = :firstName, "
    "current_last_name = :lastName, "
    "birth_date = :birthDate, "
    "house_id = :houseId, "
    "blood_status = :bloodStatus "
    "WHERE id = :id";

} // namespace SqlQueries
