#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>

void connectToDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // The database is in the 'reference_materials' subdirectory
    QString dbPath = "reference_materials/FanacalCharacters.db";
    db.setDatabaseName(dbPath);

    if (!QFile::exists(dbPath)) {
        qWarning() << "Database file does not exist at" << dbPath;
        return;
    }

    if (!db.open()) {
        qWarning() << "Error: connection with database failed:" << db.lastError().text();
    } else {
        qInfo() << "Database: connection ok";
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    connectToDatabase();

    // For this first step, we don't need to run the event loop.
    // We just want to test the connection.
    // In the future, we will add GUI elements and call a.exec().

    return 0;
}
