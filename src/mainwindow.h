#pragma once

#include <QMainWindow>
#include <QTableView>
#include <QSqlQueryModel>
#include <QSplitter>
#include <QTextEdit> // 詳細表示用
#include "CharacterService.h" // CharacterServiceを使うため

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_characterTableView_clicked(const QModelIndex &index); // キャラクターリストの行がクリックされた時のスロット

private:
    void setupDatabaseConnection(); // データベース接続のセットアップ
    void setupUI(); // UIのセットアップ
    void loadCharacterList(); // キャラクターリストの読み込み

    QTableView *characterTableView;
    QSqlQueryModel *characterListModel;
    QTextEdit *characterDetailsTextEdit; // キャラクター詳細表示用
    CharacterService *characterService; // CharacterServiceのインスタンス
};