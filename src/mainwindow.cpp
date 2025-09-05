#include "mainwindow.h"
#include "CharacterService.h"
#include "addcharacterdialog.h"
#include <QHeaderView>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QTextEdit>
#include <QSqlQueryModel>
#include <QSplitter>
#include <QSqlDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Sruighlea - Character Database");
    resize(800, 600);

    characterService = new CharacterService(this); // CharacterServiceのインスタンス化
    setupUI(); // UIのセットアップ
    loadCharacterList(); // キャラクターリストの読み込み
}

MainWindow::~MainWindow()
{
    // Qtの親子関係により、characterServiceは自動的に削除されるので、deleteは不要
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setObjectName("searchLineEdit");
    searchLineEdit->setPlaceholderText("Search by name...");
    mainLayout->addWidget(searchLineEdit);

    addCharacterButton = new QPushButton("Add New Character", this);
    addCharacterButton->setObjectName("addCharacterButton");
    mainLayout->addWidget(addCharacterButton);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(splitter);

    setupCharacterListView(splitter);
    setupCharacterDetailView(splitter);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    connect(characterTableView, &QTableView::clicked,
            this, &MainWindow::on_characterTableView_clicked);
    connect(searchLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::on_searchTextChanged);
    connect(addCharacterButton, &QPushButton::clicked,
            this, &MainWindow::on_addCharacterButton_clicked);
    connect(editCharacterButton, &QPushButton::clicked,
            this, &MainWindow::on_editCharacterButton_clicked);
}

void MainWindow::setupCharacterListView(QSplitter *splitter)
{
    characterTableView = new QTableView(this);
    characterListModel = new QSqlQueryModel(this);
    characterTableView->setModel(characterListModel);
    characterTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    characterTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    characterTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    splitter->addWidget(characterTableView);
}

void MainWindow::setupCharacterDetailView(QSplitter *splitter)
{
    characterDetailsView = new QWidget(this);
    QVBoxLayout *detailLayout = new QVBoxLayout(characterDetailsView);
    detailLayout->setContentsMargins(0, 0, 0, 0);

    characterDetailsTextEdit = new QTextEdit(this);
    characterDetailsTextEdit->setReadOnly(true);
    detailLayout->addWidget(characterDetailsTextEdit);

    editCharacterButton = new QPushButton("Edit Character", this);
    editCharacterButton->setObjectName("editCharacterButton");
    editCharacterButton->setEnabled(false);
    detailLayout->addWidget(editCharacterButton);

    splitter->addWidget(characterDetailsView);
}

void MainWindow::loadCharacterList()
{
    QSqlQueryModel* newModel = characterService->getCharacterListModel();
    updateCharacterViewModel(newModel);
}

void MainWindow::on_characterTableView_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        characterDetailsTextEdit->clear();
        editCharacterButton->setEnabled(false);
        return;
    }

    editCharacterButton->setEnabled(true);

    int characterId = characterListModel->data(characterListModel->index(index.row(), 0)).toInt();
    CharacterData details = characterService->getCharacterDetails(characterId);

    if (details.isValid()) {
        QString detailText;
        detailText += QString("ID: %1\n").arg(details.id);
        detailText += QString("First Name: %1\n").arg(details.firstName);
        detailText += QString("Last Name: %1\n").arg(details.lastName);
        detailText += QString("Birth Date: %1\n").arg(details.birthDate.toString(Qt::ISODate));
        detailText += QString("House: %1\n").arg(details.house);
        detailText += QString("Blood Status: %1\n").arg(details.bloodStatus);

        characterDetailsTextEdit->setText(detailText);
    } else {
        characterDetailsTextEdit->setText("Character details not found.");
    }
}

void MainWindow::on_searchTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        loadCharacterList();
        return;
    }

    QSqlQueryModel* newModel = characterService->searchCharactersByName(text);
    updateCharacterViewModel(newModel);
}

void MainWindow::on_addCharacterButton_clicked()
{
    AddCharacterDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        CharacterData newChar = dialog.getCharacterData();
        if (characterService->addCharacter(newChar)) {
            loadCharacterList(); // Refresh the list
        } else {
            QMessageBox::warning(this, "Error", "Failed to add character to the database.");
        }
    }
}

void MainWindow::on_editCharacterButton_clicked()
{
    // Get selected character's ID
    QModelIndexList selection = characterTableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        // This case should ideally not be reachable if the button is disabled correctly
        return;
    }
    QModelIndex idIndex = characterListModel->index(selection.first().row(), 0);
    int characterId = characterListModel->data(idIndex).toInt();

    // Fetch full data
    CharacterData currentData = characterService->getCharacterDetails(characterId);
    if (!currentData.isValid()) {
        QMessageBox::critical(this, "Error", "Could not fetch character details.");
        return;
    }

    // Open dialog with data
    AddCharacterDialog dialog(currentData, this);
    if (dialog.exec() == QDialog::Accepted) {
        CharacterData updatedData = dialog.getCharacterData();
        if (characterService->updateCharacter(updatedData)) {
            loadCharacterList(); // Refresh list
        } else {
            QMessageBox::critical(this, "Error", "Failed to update character.");
        }
    }
}

void MainWindow::updateCharacterViewModel(QSqlQueryModel *newModel)
{
    characterTableView->setModel(newModel);

    if (characterListModel) {
        characterListModel->deleteLater();
    }
    characterListModel = newModel;
    characterListModel->setParent(this);

    characterListModel->setHeaderData(1, Qt::Horizontal, tr("First Name"));
    characterListModel->setHeaderData(2, Qt::Horizontal, tr("Last Name"));
    characterListModel->setHeaderData(3, Qt::Horizontal, tr("House"));
    characterListModel->setHeaderData(4, Qt::Horizontal, tr("Blood Status"));
    characterTableView->setColumnHidden(0, true);
}