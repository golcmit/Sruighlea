#include "test_mainwindow.h"
#include "../src/mainwindow.h"
#include "../src/DatabaseManager.h"
#include <QTest>
#include <QLineEdit>
#include <QTableView>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QPushButton>
#include <QApplication>
#include <QDialog>
#include <QItemSelectionModel>

void TestMainWindow::initTestCase()
{
    // The database connection is now managed by test_main.cpp
    mainWindow = new MainWindow();
    mainWindow->show();
}

void TestMainWindow::cleanupTestCase()
{
    delete mainWindow;
    // The database connection is now managed by test_main.cpp
}

void TestMainWindow::testSearchFunctionality_WhenTextEntered_FiltersList()
{
    // ARRANGE
    QLineEdit *searchLineEdit = mainWindow->findChild<QLineEdit*>("searchLineEdit");
    QTableView *tableView = mainWindow->findChild<QTableView*>();

    QVERIFY2(searchLineEdit, "Could not find a QLineEdit named 'searchLineEdit'");
    QVERIFY2(tableView, "Could not find the QTableView");

    // Wait for the initial list to load
    QTest::qWait(500);

    QSqlQueryModel *initialModel = qobject_cast<QSqlQueryModel*>(tableView->model());
    QVERIFY2(initialModel, "The initial model should not be null");
    int initialRowCount = initialModel->rowCount();
    QVERIFY2(initialRowCount > 1, "Initial row count should be greater than 1 to test filtering.");

    // ACT
    QTest::keyClicks(searchLineEdit, "Gowin");
    QTest::qWait(500); // Wait for filtering to apply

    // ASSERT
    // The model is replaced, so we must get it again from the view.
    QSqlQueryModel *filteredModel = qobject_cast<QSqlQueryModel*>(tableView->model());
    QVERIFY2(filteredModel, "The filtered model should not be null");
    QVERIFY2(filteredModel != initialModel, "A new model should have been set");

    int filteredRowCount = filteredModel->rowCount();
    QVERIFY2(filteredRowCount < initialRowCount, "Row count should decrease after filtering.");
    QVERIFY2(filteredRowCount > 0, "Filtered row count should be greater than 0.");

    bool foundMatch = false;
    for (int i = 0; i < filteredRowCount; ++i) {
        if (filteredModel->record(i).value("current_last_name").toString() == "Gowin") {
            foundMatch = true;
            break;
        }
    }
    QVERIFY2(foundMatch, "Filtered results should contain the searched name.");
}

void TestMainWindow::testAddCharacterButton_ButtonExists()
{
    // ARRANGE
    QPushButton *addButton = mainWindow->findChild<QPushButton*>("addCharacterButton");

    // ASSERT
    QVERIFY2(addButton, "Could not find a QPushButton named 'addCharacterButton'");
}

void TestMainWindow::testEditCharacterButton_ButtonExists()
{
    // ARRANGE
    QPushButton *editButton = mainWindow->findChild<QPushButton*>("editCharacterButton");

    // ASSERT
    QVERIFY2(editButton, "Could not find a QPushButton named 'editCharacterButton'");
}

void TestMainWindow::testEditButton_InitiallyDisabled()
{
    // ARRANGE
    QPushButton *editButton = mainWindow->findChild<QPushButton*>("editCharacterButton");
    QVERIFY2(editButton, "Could not find a QPushButton named 'editCharacterButton'");

    // ASSERT
    QVERIFY2(!editButton->isEnabled(), "The edit button should be disabled initially.");
}

void TestMainWindow::testEditCharacterButton_WhenCharacterSelectedAndClicked_OpensEditDialog()
{
    // ARRANGE
    QTableView *tableView = mainWindow->findChild<QTableView*>();
    QVERIFY2(tableView, "Could not find the QTableView");
    QItemSelectionModel *selectionModel = tableView->selectionModel();
    QVERIFY2(selectionModel, "Could not get the selection model");

    // Wait for the model to be populated
    QTest::qWait(500);
    if (tableView->model()->rowCount() == 0) {
        QSKIP("Table has no rows to select, skipping test.", SkipSingle);
    }

    // ACT - Part 1: Select a row and check button state
    tableView->selectRow(0);
    QVERIFY2(selectionModel->hasSelection(), "A row should be selected");

    QPushButton *editButton = mainWindow->findChild<QPushButton*>("editCharacterButton");
    QVERIFY2(editButton, "Could not find a QPushButton named 'editCharacterButton'");

    // This part of the test might pass if the logic is already there
    QVERIFY2(editButton->isEnabled(), "The edit button should become enabled after selection.");

    // ACT - Part 2: Click the button
    QTest::mouseClick(editButton, Qt::LeftButton);
    QTest::qWait(100); // Give time for the dialog to appear

    // ASSERT
    QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
    QDialog *editDialog = nullptr;
    for (QWidget *widget : topLevelWidgets) {
        // Find a QDialog that is not the main window itself
        if (widget != mainWindow && qobject_cast<QDialog*>(widget)) {
            editDialog = qobject_cast<QDialog*>(widget);
            break;
        }
    }
    QVERIFY2(editDialog, "An edit dialog should have been opened after clicking the edit button.");

    // Cleanup: close the dialog if it was opened to not interfere with other tests
    if (editDialog) {
        editDialog->close();
    }
}
