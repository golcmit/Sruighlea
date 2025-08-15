#include "mainwindow.h"
#include "CharacterService.h"
#include <QHeaderView>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QWidget>
#include <QLineEdit>
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

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    characterTableView = new QTableView(this);
    characterListModel = new QSqlQueryModel(this);
    characterTableView->setModel(characterListModel);
    characterTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    characterTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    characterTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    splitter->addWidget(characterTableView);

    characterDetailsTextEdit = new QTextEdit(this);
    characterDetailsTextEdit->setReadOnly(true);
    splitter->addWidget(characterDetailsTextEdit);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    mainLayout->addWidget(splitter);

    connect(characterTableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::on_characterTableView_clicked);
    connect(searchLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::on_searchTextChanged);
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
        return;
    }

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
