#include <QApplication>
#include "src/mainwindow.h"
#include "src/DatabaseManager.h"
#include "src/Logger.h"
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Initialize the logger
    Logger::instance().setLogFile("Sruighlea.log");
    Logger::instance().info("Application starting.");

    if (!DatabaseManager::instance().openDatabase("reference_materials/FanacalCharacters.db")) {
        Logger::instance().critical("Could not connect to the database. The application will now exit.");
        QMessageBox::critical(nullptr, "Application Error", "Could not connect to the database. The application will now exit.");
        return 1;
    }

    MainWindow window; // MainWindowのインスタンスを作成
    window.show(); // ウィンドウを表示

    int appResult = a.exec();

    DatabaseManager::instance().closeDatabase();

    return appResult;
}
