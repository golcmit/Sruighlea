#pragma once

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    DatabaseManager(const DatabaseManager&) = delete;
    void operator=(const DatabaseManager&) = delete;

    bool openDatabase(const QString& path);
    QSqlDatabase database(); // Method to get the database connection
    void closeDatabase();

private:
    DatabaseManager();
    ~DatabaseManager() = default;
};
