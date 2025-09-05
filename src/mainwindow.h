#include <QMainWindow>

class QTableView;
class QTextEdit;
class QSqlQueryModel;
class CharacterService;
class QLineEdit;
class QPushButton;
class QSplitter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_characterTableView_clicked(const QModelIndex &index);
    void on_searchTextChanged(const QString &text);
    void on_addCharacterButton_clicked();
    void on_editCharacterButton_clicked();

private:
    void setupUI();
    void setupCharacterListView(QSplitter *splitter);
    void setupCharacterDetailView(QSplitter *splitter);
    void loadCharacterList();
    void updateCharacterViewModel(QSqlQueryModel *newModel);

    QTableView *characterTableView;
    QWidget *characterDetailsView; // Container for text edit and button
    QTextEdit *characterDetailsTextEdit;
    QPushButton *editCharacterButton;
    QSqlQueryModel *characterListModel;
    CharacterService *characterService;
    QLineEdit *searchLineEdit;
    QPushButton *addCharacterButton;
};
