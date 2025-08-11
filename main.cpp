#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QHeaderView>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>

// Function to establish a connection to the database
bool connectToDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("reference_materials/FanacalCharacters.db");

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error",
                              "Failed to connect to the database: " + db.lastError().text());
        return false;
    }
    qDebug() << "Database: connection ok";
    return true;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Create and set up the main window
    QMainWindow window;
    window.setWindowTitle("Sruighlea - Character Database");
    window.resize(800, 600);

    // Connect to the database
    if (!connectToDatabase()) {
        return -1; // Exit if connection fails
    }

    // Create a model to query the database
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT c.first_name, c.current_last_name, h.name AS house, c.blood_status FROM characters c LEFT JOIN houses h ON c.house_id = h.id");

    // Set headers for the table view
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("First Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Last Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("House"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Blood Status"));

    // Create a table view to display the data
    QTableView *view = new QTableView;
    view->setModel(model);
    
    // Set the resize mode for the horizontal header to stretch
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set the table view as the central widget of the main window
    window.setCentralWidget(view);

    // Show the main window
    window.show();

    return a.exec();
}
