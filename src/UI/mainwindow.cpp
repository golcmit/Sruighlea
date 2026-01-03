#include "mainwindow.h"
#include "../Services/CharacterService.h"
//#include "addcharacterdialog.h"
#include <QHeaderView>
#include <QSqlError>
#include <QMessageBox>
#include "../Core/Logger.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QTabWidget>
#include <QSqlQueryModel>
#include <QSplitter>
#include <QSqlDatabase>
#include <QFormLayout>
#include <QLabel>
#include <QDateEdit>
#include <QTextEdit>
#include <QDebug>
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
    auto connection=connect(characterTableView, &QTableView::clicked,
            this, &MainWindow::on_characterTableView_clicked);

    /*
    connect(searchLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::on_searchTextChanged);
    connect(addCharacterButton, &QPushButton::clicked,
            this, &MainWindow::on_addCharacterButton_clicked);
    connect(editCharacterButton, &QPushButton::clicked,
            this, &MainWindow::on_editCharacterButton_clicked);
    */
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



void MainWindow::setupBasicInfoTab()
{
    qDebug() << "BasicInfoTab: Start";
    // 1. タブ本体となるウィジェットとレイアウトを作成
    basicInfoTab = new QWidget(this);
    basicInfoLayout = new QFormLayout(basicInfoTab); // フォームレイアウト
    qDebug() << "BasicInfoTab: Widgets creating...";
    basicInfoLayout->setContentsMargins(10, 10, 10, 10);
    basicInfoLayout->setSpacing(10);

    // 2. 各フィールドのウィジェットをインスタンス化
    fullNameLineEdit = new QLineEdit(this);
    sortNameLineEdit = new QLineEdit(this);
    bloodStatusLineEdit = new QLineEdit(this);
    patronusLineEdit = new QLineEdit(this);
    speciesLineEdit = new QLineEdit(this);
    birthDateEdit = new QDateEdit(this);
    birthDateEdit->setCalendarPopup(true);
    deathDateEdit = new QDateEdit(this);
    deathDateEdit->setCalendarPopup(true);
    schoolLineEdit = new QLineEdit(this);
    houseLineEdit = new QLineEdit(this);
    lineageLineEdit = new QLineEdit(this);
    

    // 複数行テキスト
    wandTextEdit = new QTextEdit(this);
    wandTextEdit->setFixedHeight(80); // 高さを固定 (例)
    notesTextEdit = new QTextEdit(this);
    notesTextEdit->setFixedHeight(100); // 高さを固定 (例)
    qDebug() << "BasicInfoTab: Row adding...";
    // ★ WBS 2.0 (閲覧フェーズ) のため、すべて読み取り専用に設定
    fullNameLineEdit->setReadOnly(true);
    sortNameLineEdit->setReadOnly(true);
    bloodStatusLineEdit->setReadOnly(true);
    patronusLineEdit->setReadOnly(true);
    speciesLineEdit->setReadOnly(true);
    birthDateEdit->setReadOnly(true);
    deathDateEdit->setReadOnly(true);
    wandTextEdit->setReadOnly(true);
    notesTextEdit->setReadOnly(true);
    schoolLineEdit->setReadOnly(true);
    houseLineEdit->setReadOnly(true);
    lineageLineEdit->setReadOnly(true);



    // 3. レイアウトにウィジェットを追加
    basicInfoLayout->addRow(new QLabel("Full Name:", this), fullNameLineEdit);
    basicInfoLayout->addRow(new QLabel("Sort Name:", this), sortNameLineEdit);
    basicInfoLayout->addRow(new QLabel("Birth Date:", this), birthDateEdit);
    basicInfoLayout->addRow(new QLabel("Death Date:", this), deathDateEdit);
    basicInfoLayout->addRow(new QLabel("Blood Status:", this), bloodStatusLineEdit);
    basicInfoLayout->addRow(new QLabel("Patronus:", this), patronusLineEdit);
    basicInfoLayout->addRow(new QLabel("Species:", this), speciesLineEdit);
    basicInfoLayout->addRow(new QLabel("Wand (JSON):", this), wandTextEdit);
    basicInfoLayout->addRow(new QLabel("Notes:", this), notesTextEdit);
        // レイアウトへの追加（お好みの位置に）
    basicInfoLayout->addRow(new QLabel("School:", this), schoolLineEdit);
    basicInfoLayout->addRow(new QLabel("House:", this), houseLineEdit);
    basicInfoLayout->addRow(new QLabel("Lineage:", this), lineageLineEdit);

    // 4. QTabWidget にこのタブを追加
    qDebug() << "BasicInfoTab: Adding to mainTabWidget. mainTabWidget is:" << mainTabWidget;

    
    if (mainTabWidget) {
        mainTabWidget->addTab(basicInfoTab, "Basic Info");
    } else {
        qDebug() << "CRITICAL: mainTabWidget is NULL!";
    }

    qDebug() << "BasicInfoTab: End";
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

    

    // 1. QTabWidget のインスタンスを作成
    mainTabWidget = new QTabWidget(this);
    mainTabWidget->setObjectName("mainTabWidget");

    // 2. レイアウトに QTabWidget を追加
    detailLayout->addWidget(mainTabWidget);
    setupBasicInfoTab();
    setupCareerTab();
    splitter->addWidget(characterDetailsView);
}

void MainWindow::setupCareerTab()
{
    careerTab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(careerTab);

    layout->addWidget(new QLabel("Academic Records (Years/Subjects):", this));
    academicDisplayText = new QTextEdit(this);
    academicDisplayText->setReadOnly(true);
    layout->addWidget(academicDisplayText);

    layout->addWidget(new QLabel("Occupation History:", this));
    occupationDisplayText = new QTextEdit(this);
    occupationDisplayText->setReadOnly(true);
    layout->addWidget(occupationDisplayText);

    mainTabWidget->addTab(careerTab, "Career");
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
 * @brief 基本情報タブの表示更新
 */
void MainWindow::displayBasicInfo(const Character &details)
{
    fullNameLineEdit->setText(details.fullName);
    sortNameLineEdit->setText(details.sortName);
    bloodStatusLineEdit->setText(details.bloodStatus);
    patronusLineEdit->setText(details.patronus);
    speciesLineEdit->setText(details.species);
    birthDateEdit->setDate(details.birthDate);
    deathDateEdit->setDate(details.deathDate);
    wandTextEdit->setPlainText(details.wand);
    notesTextEdit->setPlainText(details.notes);
    schoolLineEdit->setText(details.schoolName);
    houseLineEdit->setText(details.houseName);
    lineageLineEdit->setText(details.lineageName);
}

/**
 * @brief 経歴（学業・職歴）タブの表示更新
 */
void MainWindow::displayCareer(const Character &details)
{
    // 学業情報の整形
    QString acText;
    for (const auto &ac : details.academics) {
        QString exam = ac.isOwl ? " [O.W.L.]" : (ac.isNewt ? " [N.E.W.T.]" : "");
        acText += QString("Year %1: %2 - Grade: %3%4\n")
                    .arg(ac.academicYear).arg(ac.subjectName).arg(ac.grade).arg(exam);
    }
    academicDisplayText->setPlainText(acText);

    // 職歴情報の整形
    QString ocText;
    for (const auto &oc : details.occupations) {
        QString start = oc.startDate.toString("yyyy");
        QString end = oc.endDate.isValid() ? oc.endDate.toString("yyyy") : "Present";
        ocText += QString("%1 at %2 (%3 - %4)\n")
                    .arg(oc.occupation).arg(oc.organization).arg(start).arg(end);
    }
    occupationDisplayText->setPlainText(ocText);
}

/**
 * @brief 人間関係タブの表示更新
 */
void MainWindow::displayRelationships(const Character &details)
{
    // ここに QSqlQuery はもういりません
    QString relText;
    for (const auto &rel : details.relationships) {
        relText += QString("・%1 : %2\n").arg(rel.relationshipTypeName).arg(rel.toCharacterName);
        if (!rel.notes.isEmpty()) {
            relText += QString("   (備考: %1)\n").arg(rel.notes);
        }
    }
    // MainWindow.h に relationDisplayText を追加した前提です
    if (relationDisplayText) {
        relationDisplayText->setPlainText(relText);
    }
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
    if (!index.isValid()) return;
    
    // 1. IDの取得
    int characterId = characterListModel->data(characterListModel->index(index.row(), 0)).toInt();
  
    // 2. サービス層から全データ(経歴・関係性含む)をワンショットで取得
    Character details = characterService->getCharacterDetails(characterId);
   
    // 3. UIへの反映（各表示メソッドに委譲）
    if (details.isValid()) {
        displayBasicInfo(details);
        displayCareer(details);
        displayRelationships(details);
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
    /*
    // 検索テキストが空の場合は全リストを表示
    if (text.isEmpty()) {
        loadCharacterList();
        return;
    }

    // 名前で検索してモデルを更新
    QSqlQueryModel* newModel = characterService->searchCharactersByName(text);
    updateCharacterViewModel(newModel);
    */
}

/**
 * @brief キャラクター追加ボタンがクリックされた時の処理
 * 
 * 追加ダイアログを表示し、OKが押された場合は
 * 新しいキャラクターをデータベースに追加してリストを更新する
 */
void MainWindow::on_addCharacterButton_clicked()
{
    /*
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
        */
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
    /*
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
*/
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
    if (!newModel) return;

    if (characterListModel) {
        characterListModel->deleteLater();
    }
    characterListModel = newModel;
    
    // 1. モデルをセット
    characterTableView->setModel(characterListModel);

    // 2. 一旦、すべての設定をリセット（コメントアウトではなく、あえて「見える」設定にする）
    for (int i = 0; i < characterListModel->columnCount(); ++i) {
        characterTableView->setColumnHidden(i, false); // すべて表示
    }
    
    // 3. ヘッダーを表示して幅を自動調整
    characterTableView->horizontalHeader()->setVisible(true);
    characterTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // デバッグ：これで見えなかったら View 自体がおかしい
    Logger::instance().debug("Model set to TableView. Column count: " + QString::number(characterListModel->columnCount()));
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
    // editCharacterButton->setEnabled(!selectedRows.isEmpty());
}