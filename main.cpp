// 必要なヘッダーファイルをインクルード
#include <QApplication>      // Qtアプリケーションの管理クラス
#include "src/UI/mainwindow.h"  // メインウィンドウのクラス
#include "src/Core/DatabaseManager.h" // データベース管理クラス
#include "src/Core/Logger.h"      // ログ出力クラス
#include <QMessageBox>       // メッセージボックス表示クラス

// main関数：アプリケーションのエントリーポイント（ここからプログラムが始まる）
int main(int argc, char *argv[]) {
    // Qtアプリケーションのインスタンスを生成
    QApplication a(argc, argv);

    // --- ロガーの初期化 ---
    // ログファイルのパスを設定
    Logger::instance().setLogFile("Sruighlea.log");
    // アプリケーション開始のログを記録
    Logger::instance().info("Application starting.");

    // --- データベース接続 ---
    // データベースファイルを開く試み
    if (!DatabaseManager::instance().openDatabase("neo_design/new_FanacalCharacters.db")) {
        // 接続に失敗した場合
        // 致命的なエラーとしてログを記録
        Logger::instance().critical("Could not connect to the database. The application will now exit.");
        // ユーザーにエラーメッセージボックスを表示
        QMessageBox::critical(nullptr, "Application Error", "Could not connect to the database. The application will now exit.");
        // エラーコード1でアプリケーションを終了
        return 1;
    }

    // --- メインウィンドウの表示 ---
    MainWindow window; // MainWindowのインスタンスを作成
    window.show();     // ウィンドウを表示

    // --- アプリケーションの実行 ---
    // Qtのイベントループを開始し、ウィンドウが表示され続けるようにする
    // ウィンドウが閉じられるなど、終了イベントが発生するまで待機する
    int appResult = a.exec();

    // --- 終了処理 ---
    // データベース接続を閉じる
    DatabaseManager::instance().closeDatabase();

    // アプリケーションの実行結果（終了コード）を返す
    return appResult;
}