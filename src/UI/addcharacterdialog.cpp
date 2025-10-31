// 必要なヘッダーファイルをインクルード
#include "addcharacterdialog.h" // このダイアログ自身のヘッダー
#include <QVBoxLayout> // 垂直レイアウト
#include <QFormLayout> // フォームレイアウト
#include <QLineEdit> // 1行テキスト入力
#include <QComboBox> // ドロップダウンリスト
#include <QDateEdit> // 日付入力
#include <QPushButton> // プッシュボタン
#include <QDialogButtonBox> // OK/Cancelなどの標準的なボタンボックス

// 新規キャラクター追加用のコンストラクタ
AddCharacterDialog::AddCharacterDialog(QWidget *parent)
    : QDialog(parent) // 親クラスのコンストラクタを呼び出す
{
    setWindowTitle("Add New Character"); // ウィンドウのタイトルを設定
    setupUI(); // UIのセットアップ関数を呼び出す
}

// 既存キャラクター編集用のコンストラクタ
AddCharacterDialog::AddCharacterDialog(const CharacterData &editData, QWidget *parent)
    : QDialog(parent) // 親クラスのコンストラクタを呼び出す
{
    setWindowTitle("Edit Character"); // ウィンドウのタイトルを設定
    setupUI(); // UIのセットアップ関数を呼び出す
    populateFields(editData); // 渡されたデータでフィールドを埋める
}

// UIのセットアップを行う関数
void AddCharacterDialog::setupUI()
{
    // フォーム形式のレイアウトを作成
    QFormLayout *formLayout = new QFormLayout();

    // 各UIウィジェットを生成
    firstNameEdit = new QLineEdit(this); // 名前の入力欄
    lastNameEdit = new QLineEdit(this); // 姓の入力欄
    houseComboBox = new QComboBox(this); // 寮の選択ボックス
    bloodStatusComboBox = new QComboBox(this); // 血統の選択ボックス
    birthDateEdit = new QDateEdit(this); // 生年月日の入力欄
    birthDateEdit->setDate(QDate::currentDate()); // デフォルト値を現在の日付に設定
    birthDateEdit->setCalendarPopup(true); // カレンダーポップアップを有効化

    // コンボボックスに選択肢を追加（本来はデータベースなどから取得するべき）
    houseComboBox->addItems({"gryffindor", "hufflepuff", "ravenclaw", "slytherin"});
    bloodStatusComboBox->addItems({"pure", "half", "muggleborn"});

    // フォームレイアウトに各ウィジェットを追加
    formLayout->addRow("First Name:", firstNameEdit);
    formLayout->addRow("Last Name:", lastNameEdit);
    formLayout->addRow("House:", houseComboBox);
    formLayout->addRow("Blood Status:", bloodStatusComboBox);
    formLayout->addRow("Birth Date:", birthDateEdit);

    // 保存(Save)ボタンとキャンセル(Cancel)ボタンを持つボタンボックスを生成
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    saveButton = buttonBox->button(QDialogButtonBox::Save); // 保存ボタンへのポインタを取得
    cancelButton = buttonBox->button(QDialogButtonBox::Cancel); // キャンセルボタンへのポインタを取得

    // ボタンボックスのシグナルとダイアログのスロットを接続
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept); // OK/Saveが押されたらaccept()を呼ぶ
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject); // Cancelが押されたらreject()を呼ぶ

    // メインの垂直レイアウトを生成
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout); // フォームレイアウトを追加
    mainLayout->addWidget(buttonBox); // ボタンボックスを追加
}

// 編集時に既存のキャラクターデータでフォームを埋める関数
void AddCharacterDialog::populateFields(const CharacterData &data)
{
    editingCharacterId = data.id; // 編集中のキャラクターIDを保持
    firstNameEdit->setText(data.firstName); // 名前の入力欄にセット
    lastNameEdit->setText(data.lastName); // 姓の入力欄にセット
    houseComboBox->setCurrentText(data.house); // 寮の選択ボックスにセット
    bloodStatusComboBox->setCurrentText(data.bloodStatus); // 血統の選択ボックスにセット
    birthDateEdit->setDate(data.birthDate); // 生年月日の入力欄にセット
}

// 入力されたキャラクターデータを取得する関数
CharacterData AddCharacterDialog::getCharacterData() const
{
    CharacterData data; // 戻り値用のデータ構造体
    data.id = editingCharacterId; // 編集中のIDをセット（新規の場合は初期値のまま）
    data.firstName = firstNameEdit->text(); // 名前の入力内容を取得
    data.lastName = lastNameEdit->text(); // 姓の入力内容を取得
    data.house = houseComboBox->currentText(); // 寮の選択内容を取得
    data.bloodStatus = bloodStatusComboBox->currentText(); // 血統の選択内容を取得
    data.birthDate = birthDateEdit->date(); // 生年月日の入力内容を取得
    return data; // データを返す
}