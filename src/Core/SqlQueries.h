#pragma once

#include <QString>

namespace SqlQueries {

constexpr const char* GET_CHARACTER_DETAILS =
    "SELECT c.id, c.first_name, c.current_last_name, c.birth_date, h.name AS house_name, c.blood_status "
    "FROM characters c LEFT JOIN houses h ON c.house_id = h.id "
    "WHERE c.id = :id";

constexpr const char* GET_CHARACTER_LIST =
    "SELECT id, full_name, sort_name FROM characters ORDER BY sort_name";

constexpr const char* SEARCH_CHARACTERS_BY_NAME =
    "SELECT c.id, c.first_name, c.current_last_name, h.name AS house, c.blood_status "
    "FROM characters c LEFT JOIN houses h ON c.house_id = h.id "
    "WHERE c.first_name LIKE :name OR c.current_last_name LIKE :name "
    "ORDER BY c.id";

constexpr const char* GET_HOUSE_ID_BY_NAME =
    "SELECT id FROM houses WHERE name = :name";

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
