// 必要なヘッダーファイルをインクルード
#include "DatabaseManager.h" // このクラス自身のヘッダー
#include "Logger.h" // ログ出力用のクラス
#include <QSqlError> // Qt SQLのエラー情報を扱うクラス

// シングルトンインスタンスを取得するための静的関数
// この関数を通じてのみ、DatabaseManagerの唯一のインスタンスにアクセスできる
DatabaseManager& DatabaseManager::instance()
{
    // 静的ローカル変数としてインスタンスを生成する
    // これにより、最初に呼び出されたときに一度だけインスタンスが作成される
    static DatabaseManager dm_instance;
    return dm_instance; // インスタンスへの参照を返す
}

// コンストラクタ（デフォルト実装を使用）
// シングルトンパターンなので、外部から直接インスタンス化されることはない
DatabaseManager::DatabaseManager() = default;

// データベース接続を開く関数
bool DatabaseManager::openDatabase(const QString& path)
{
    // 使用するデータベースドライバとしてSQLiteを指定し、データベース接続を追加
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // データベースファイルのパスを設定
    db.setDatabaseName(path);

    // データベースを開く
    if (!db.open()) {
        // 接続に失敗した場合、エラーログを出力
        Logger::instance().error("DatabaseManager Error: " + db.lastError().text());
        return false; // 失敗を示すfalseを返す
    }

    // 接続に成功した場合、情報ログを出力
    Logger::instance().info("Database connection established successfully.");
    return true; // 成功を示すtrueを返す
}

// 現在のデータベース接続オブジェクトを取得する関数
QSqlDatabase DatabaseManager::database()
{
    // デフォルトのデータベース接続を返す
    return QSqlDatabase::database();
}

// データベース接続を閉じる関数
void DatabaseManager::closeDatabase()
{
    // 現在の接続名を取得
    QString connectionName = QSqlDatabase::database().connectionName();
    // データベースが開いているか確認
    if (QSqlDatabase::database().isOpen()) {
        // 開いていれば閉じる
        QSqlDatabase::database().close();
    }
    // データベース接続を削除し、リソースを解放する
    QSqlDatabase::removeDatabase(connectionName);
}