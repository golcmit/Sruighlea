#include <QApplication>
#include "src/mainwindow.h" // 新しいMainWindowをインクルード

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow window; // MainWindowのインスタンスを作成
    window.show(); // ウィンドウを表示

    return a.exec();
}
