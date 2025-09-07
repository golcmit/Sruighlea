#include "DatabaseManager.h"
#include "Logger.h"
#include <QSqlError>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager dm_instance;
    return dm_instance;
}

DatabaseManager::DatabaseManager() = default;

bool DatabaseManager::openDatabase(const QString& path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        Logger::instance().error("DatabaseManager Error: " + db.lastError().text());
        return false;
    }

    Logger::instance().info("Database connection established successfully.");
    return true;
}

QSqlDatabase DatabaseManager::database()
{
    return QSqlDatabase::database();
}

void DatabaseManager::closeDatabase()
{
    QString connectionName = QSqlDatabase::database().connectionName();
    if (QSqlDatabase::database().isOpen()) {
        QSqlDatabase::database().close();
    }
    QSqlDatabase::removeDatabase(connectionName);
}
