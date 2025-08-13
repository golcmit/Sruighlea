#include "mainwindow.h"
#include <QHeaderView>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Sruighlea - Character Database");
    resize(800, 600);

    setupDatabaseConnection(); // データベース接続
    characterService = new CharacterService(this); // CharacterServiceのインスタンス化
    setupUI(); // UIのセットアップ
    loadCharacterList(); // キャラクターリストの読み込み
}

MainWindow::~MainWindow()
{
    // デストラクタでCharacterServiceを削除 (親オブジェクトが管理するので通常は不要だが、明示的に)
    delete characterService;
    // データベース接続はQSqlDatabaseが管理するので、ここでは閉じない
}

void MainWindow::setupDatabaseConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("reference_materials/FanacalCharacters.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error",
                              "Failed to connect to the database: " + db.lastError().text());
        // アプリケーションを終了させるか、エラーハンドリングを行う
        qFatal("Failed to connect to database!");
    }
    qDebug() << "Database: connection ok";
}

void MainWindow::setupUI()
{
    // メインのウィジェットとレイアウト
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // キャラクターリストのテーブルビュー
    characterTableView = new QTableView(this);
    characterListModel = new QSqlQueryModel(this);
    characterTableView->setModel(characterListModel);
    characterTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    characterTableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 行全体を選択
    characterTableView->setSelectionMode(QAbstractItemView::SingleSelection); // 単一行選択

    // キャラクター詳細表示用のテキストエディット
    characterDetailsTextEdit = new QTextEdit(this);
    characterDetailsTextEdit->setReadOnly(true); // 読み取り専用

    // スプリッターで左右に配置
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(characterTableView);
    splitter->addWidget(characterDetailsTextEdit);
    splitter->setStretchFactor(0, 1); // テーブルビューを広めに
    splitter->setStretchFactor(1, 1); // 詳細表示も広めに

    mainLayout->addWidget(splitter);

    // シグナルとスロットの接続
    connect(characterTableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::on_characterTableView_clicked);
}

void MainWindow::loadCharacterList()
{
    characterListModel->setQuery("SELECT c.id, c.first_name, c.current_last_name, h.name AS house, c.blood_status FROM characters c LEFT JOIN houses h ON c.house_id = h.id");

    characterListModel->setHeaderData(1, Qt::Horizontal, tr("First Name"));
    characterListModel->setHeaderData(2, Qt::Horizontal, tr("Last Name"));
    characterListModel->setHeaderData(3, Qt::Horizontal, tr("House"));
    characterListModel->setHeaderData(4, Qt::Horizontal, tr("Blood Status"));

    // IDカラムは表示しないが、選択時に取得できるようにしておく
    characterTableView->setColumnHidden(0, true);
}

void MainWindow::on_characterTableView_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        characterDetailsTextEdit->clear();
        return;
    }

    // 選択された行のIDを取得 (非表示のIDカラムから)
    int characterId = characterListModel->data(characterListModel->index(index.row(), 0)).toInt();

    // CharacterServiceを使って詳細情報を取得
    CharacterData details = characterService->getCharacterDetails(characterId);

    if (details.isValid()) {
        QString detailText;
        detailText += QString("ID: %1\n").arg(details.id);
        detailText += QString("First Name: %1\n").arg(details.firstName);
        detailText += QString("Last Name: %1\n").arg(details.lastName);
        detailText += QString("Birth Date: %1\n").arg(details.birthDate.toString(Qt::ISODate));
        detailText += QString("House: %1\n").arg(details.house);
        detailText += QString("Blood Status: %1\n").arg(details.bloodStatus);
        // 必要に応じて他のフィールドも追加

        characterDetailsTextEdit->setText(detailText);
    } else {
        characterDetailsTextEdit->setText("Character details not found.");
    }
}
