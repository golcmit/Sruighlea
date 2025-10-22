#include "mainwindow.h"
#include "CharacterService.h"
#include "addcharacterdialog.h"
#include <QHeaderView>
#include <QSqlError>
#include <QMessageBox>
#include "Logger.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QTextEdit>
#include <QSqlQueryModel>
#include <QSplitter>
#include <QSqlDatabase>

/**
 * @brief MainWindowのコンストラクタ
 * @param parent 親ウィジェット
 * 
 * ウィンドウのタイトルとサイズを設定し、
 * CharacterServiceのインスタンスを作成し、
 * UIをセットアップして、キャラクターリストを読み込む
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Sruighlea - Character Database");
    resize(800, 600);

    characterService = new CharacterService(this); // CharacterServiceのインスタンス化
    setupUI(); // UIのセットアップ
    loadCharacterList(); // キャラクターリストの読み込み
}

/**
 * @brief MainWindowのデストラクタ
 * 
 * Qtの親子関係により、characterServiceは自動的に削除されるため、
 * 明示的なdeleteは不要
 */
MainWindow::~MainWindow()
{
    // Qtの親子関係により、characterServiceは自動的に削除されるので、deleteは不要
}

/**
 * @brief UIコンポーネントのセットアップ
 * 
 * メインウィンドウのレイアウトを構築し、
 * 検索バー、追加ボタン、キャラクターリスト、詳細ビューを配置し、
 * 各ウィジェットのシグナルとスロットを接続する
 */
void MainWindow::setupUI()
{
    // 中央ウィジェットとメインレイアウトの作成
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 検索用テキストボックスの作成
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setObjectName("searchLineEdit");
    searchLineEdit->setPlaceholderText("Search by name...");
    mainLayout->addWidget(searchLineEdit);

    // キャラクター追加ボタンの作成
    addCharacterButton = new QPushButton("Add New Character", this);
    addCharacterButton->setObjectName("addCharacterButton");
    mainLayout->addWidget(addCharacterButton);

    // 水平分割ウィジェットの作成（リストと詳細を左右に配置）
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(splitter);

    // キャラクターリストビューと詳細ビューのセットアップ
    setupCharacterListView(splitter);
    setupCharacterDetailView(splitter);

    // スプリッターの各領域のサイズ比率を設定
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    // シグナルとスロットの接続
    connect(characterTableView, &QTableView::clicked,
            this, &MainWindow::on_characterTableView_clicked);
    connect(searchLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::on_searchTextChanged);
    connect(addCharacterButton, &QPushButton::clicked,
            this, &MainWindow::on_addCharacterButton_clicked);
    connect(editCharacterButton, &QPushButton::clicked,
            this, &MainWindow::on_editCharacterButton_clicked);
}

/**
 * @brief キャラクターリストビューのセットアップ
 * @param splitter 親となるスプリッターウィジェット
 * 
 * テーブルビューを作成し、SQLクエリモデルを設定し、
 * 列の自動リサイズと行選択モードを設定する
 */
void MainWindow::setupCharacterListView(QSplitter *splitter)
{
    // テーブルビューとモデルの作成
    characterTableView = new QTableView(this);
    characterListModel = new QSqlQueryModel(this);
    characterTableView->setModel(characterListModel);
    
    // 列の幅を自動調整
    characterTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // 行全体を選択する設定
    characterTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    characterTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    
    splitter->addWidget(characterTableView);
}

/**
 * @brief キャラクター詳細ビューのセットアップ
 * @param splitter 親となるスプリッターウィジェット
 * 
 * 詳細表示用のテキストエディットと編集ボタンを配置する
 * 編集ボタンは初期状態で無効化されている
 */
void MainWindow::setupCharacterDetailView(QSplitter *splitter)
{
    // 詳細ビュー用のウィジェットとレイアウトの作成
    characterDetailsView = new QWidget(this);
    QVBoxLayout *detailLayout = new QVBoxLayout(characterDetailsView);
    detailLayout->setContentsMargins(0, 0, 0, 0);

    // 詳細情報を表示する読み取り専用テキストエディット
    characterDetailsTextEdit = new QTextEdit(this);
    characterDetailsTextEdit->setReadOnly(true);
    detailLayout->addWidget(characterDetailsTextEdit);

    // キャラクター編集ボタン（初期状態では無効）
    editCharacterButton = new QPushButton("Edit Character", this);
    editCharacterButton->setObjectName("editCharacterButton");
    editCharacterButton->setEnabled(false);
    detailLayout->addWidget(editCharacterButton);

    splitter->addWidget(characterDetailsView);
}

/**
 * @brief キャラクターリストの読み込み
 * 
 * CharacterServiceからキャラクターリストのモデルを取得し、
 * ビューを更新する
 */
void MainWindow::loadCharacterList()
{
    QSqlQueryModel* newModel = characterService->getCharacterListModel();
    updateCharacterViewModel(newModel);
}

/**
 * @brief キャラクターリストのアイテムがクリックされた時の処理
 * @param index クリックされたアイテムのインデックス
 * 
 * 選択されたキャラクターの詳細情報を取得し、
 * 詳細ビューに表示する
 */
void MainWindow::on_characterTableView_clicked(const QModelIndex &index)
{
    // インデックスが無効な場合は詳細をクリア
    if (!index.isValid()) {
        characterDetailsTextEdit->clear();
        return;
    }

    // 選択された行からキャラクターIDを取得
    int characterId = characterListModel->data(characterListModel->index(index.row(), 0)).toInt();
    
    // CharacterServiceから詳細情報を取得
    CharacterData details = characterService->getCharacterDetails(characterId);

    // 詳細情報が有効な場合、フォーマットして表示
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

/**
 * @brief 検索テキストが変更された時の処理
 * @param text 入力された検索テキスト
 * 
 * テキストが空の場合は全リストを表示し、
 * テキストがある場合は名前で検索してフィルタリングする
 */
void MainWindow::on_searchTextChanged(const QString &text)
{
    // 検索テキストが空の場合は全リストを表示
    if (text.isEmpty()) {
        loadCharacterList();
        return;
    }

    // 名前で検索してモデルを更新
    QSqlQueryModel* newModel = characterService->searchCharactersByName(text);
    updateCharacterViewModel(newModel);
}

/**
 * @brief キャラクター追加ボタンがクリックされた時の処理
 * 
 * 追加ダイアログを表示し、OKが押された場合は
 * 新しいキャラクターをデータベースに追加してリストを更新する
 */
void MainWindow::on_addCharacterButton_clicked()
{
    // キャラクター追加ダイアログを表示
    AddCharacterDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // ダイアログから入力データを取得
        CharacterData newChar = dialog.getCharacterData();
        
        // データベースに追加
        if (characterService->addCharacter(newChar)) {
            loadCharacterList(); // リストを更新
        } else {
            // 追加失敗時のエラーメッセージ
            QMessageBox::warning(this, "Error", "Failed to add character to the database.");
        }
    }
}

/**
 * @brief キャラクター編集ボタンがクリックされた時の処理
 * 
 * 選択されたキャラクターの詳細を取得し、
 * 編集ダイアログを表示して更新を行う
 * テストモード時はモードレスダイアログ、通常時はモーダルダイアログを使用
 */
void MainWindow::on_editCharacterButton_clicked()
{
    // 選択された行のキャラクターIDを取得
    QModelIndexList selection = characterTableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        // 通常、ボタンが無効化されているため、この条件には到達しないはず
        return;
    }
    QModelIndex idIndex = characterListModel->index(selection.first().row(), 0);
    int characterId = characterListModel->data(idIndex).toInt();

    // キャラクターの完全な詳細データを取得
    CharacterData currentData = characterService->getCharacterDetails(characterId);
    if (!currentData.isValid()) {
        Logger::instance().error("Could not fetch character details for editing.");
        QMessageBox::critical(this, "Error", "Could not fetch character details.");
        return;
    }

#ifdef QT_TESTLIB_LIB
    // テストモード時：モードレスダイアログを開く（テストの継続を許可）
    auto* dialog = new AddCharacterDialog(currentData, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
#else
    // 通常モード時：モーダルダイアログを開く
    AddCharacterDialog dialog(currentData, this);
    if (dialog.exec() == QDialog::Accepted) {
        // ダイアログから更新されたデータを取得
        CharacterData updatedData = dialog.getCharacterData();
        
        // データベースを更新
        if (characterService->updateCharacter(updatedData)) {
            loadCharacterList(); // リストを更新
        } else {
            // 更新失敗時のエラーメッセージ
            QMessageBox::critical(this, "Error", "Failed to update character.");
        }
    }
#endif
}

/**
 * @brief キャラクタービューのモデルを更新
 * @param newModel 新しいSQLクエリモデル
 * 
 * テーブルビューに新しいモデルを設定し、
 * 列のヘッダー名を設定し、ID列を非表示にする
 * 古いモデルは削除される
 */
void MainWindow::updateCharacterViewModel(QSqlQueryModel *newModel)
{
    // テーブルビューに新しいモデルを設定
    characterTableView->setModel(newModel);
    
    // 選択状態の変更を監視
    connect(characterTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::on_characterSelectionChanged);

    // 古いモデルを後で削除（メモリリーク防止）
    if (characterListModel) {
        characterListModel->deleteLater();
    }
    
    // 新しいモデルを保存し、親を設定
    characterListModel = newModel;
    characterListModel->setParent(this);

    // 列のヘッダー名を設定
    characterListModel->setHeaderData(1, Qt::Horizontal, tr("First Name"));
    characterListModel->setHeaderData(2, Qt::Horizontal, tr("Last Name"));
    characterListModel->setHeaderData(3, Qt::Horizontal, tr("House"));
    characterListModel->setHeaderData(4, Qt::Horizontal, tr("Blood Status"));
    
    // ID列（列0）を非表示に設定
    characterTableView->setColumnHidden(0, true);
}

/**
 * @brief キャラクターの選択状態が変更された時の処理
 * 
 * 行が選択されている場合は編集ボタンを有効化し、
 * 選択されていない場合は無効化する
 */
void MainWindow::on_characterSelectionChanged()
{
    const QModelIndexList selectedRows = characterTableView->selectionModel()->selectedRows();
    editCharacterButton->setEnabled(!selectedRows.isEmpty());
}