#include "test_characterservice.h"
#include "test_mainwindow.h"
#include "test_logger.h"
#include "../src/DatabaseManager.h"
#include <QApplication>
#include <QTest>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int status = 0;

    // Logger test doesn't need the database, run it first.
    {
        TestLogger tl;
        status |= QTest::qExec(&tl, argc, argv);
    }

    // Open the database connection once for all test suites.
    if (!DatabaseManager::instance().openDatabase("reference_materials/FanacalCharacters.db")) {
        qFatal("FATAL: Could not open database for testing. Aborting.");
    }

    {
        TestCharacterService tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    {
        TestMainWindow tmw;
        status |= QTest::qExec(&tmw, argc, argv);
    }

    // Close the connection after all tests are done.
    DatabaseManager::instance().closeDatabase();

    return status;
}
