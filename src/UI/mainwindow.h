#include <QMainWindow>

class QTableView;
class QTabWidget;
class QSqlQueryModel;
class CharacterService;
class QLineEdit;
class QPushButton;
class QSplitter;
class QFormLayout;
class QLabel;
class QDateEdit;
class QTextEdit;
class QDebug;
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
    void on_characterSelectionChanged();

private:
    void setupUI();
    void setupCharacterListView(QSplitter *splitter);
    void setupCharacterDetailView(QSplitter *splitter);
    void loadCharacterList();
    void updateCharacterViewModel(QSqlQueryModel *newModel);
    void setupBasicInfoTab();
    QTableView *characterTableView =nullptr;
    QWidget *characterDetailsView =nullptr; // Container for text edit and button
    QTabWidget *mainTabWidget =nullptr;
    QSqlQueryModel *characterListModel =nullptr;
    CharacterService *characterService =nullptr;
    QLineEdit *searchLineEdit =nullptr;
    QPushButton *addCharacterButton =nullptr;
    QWidget *basicInfoTab =nullptr;
    QFormLayout *basicInfoLayout =nullptr;
    
    QLineEdit *fullNameLineEdit =nullptr;
    QLineEdit *sortNameLineEdit =nullptr;
    QLineEdit *bloodStatusLineEdit =nullptr;
    QDateEdit *birthDateEdit =nullptr;
    QDateEdit *deathDateEdit =nullptr;
    QLineEdit *patronusLineEdit =nullptr;
    QLineEdit *speciesLineEdit =nullptr;
    QTextEdit *wandTextEdit =nullptr;
    QTextEdit *notesTextEdit =nullptr;
};
