#include <QMainWindow>

class QTableView;
class QTextEdit;
class QSqlQueryModel;
class CharacterService;
class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_characterTableView_clicked(const QModelIndex &index);
    void on_searchTextChanged(const QString &text);

private:
    void setupUI();
    void loadCharacterList();
    void updateCharacterViewModel(QSqlQueryModel *newModel);

    QTableView *characterTableView;
    QTextEdit *characterDetailsTextEdit;
    QSqlQueryModel *characterListModel;
    CharacterService *characterService;
    QLineEdit *searchLineEdit;
};
