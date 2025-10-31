#pragma once

#include <QDialog>
#include "../Services/CharacterService.h" // For CharacterData

class QLineEdit;
class QComboBox;
class QDateEdit;
class QPushButton;

class AddCharacterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCharacterDialog(QWidget *parent = nullptr); // For adding
    explicit AddCharacterDialog(const CharacterData &editData, QWidget *parent = nullptr); // For editing

    CharacterData getCharacterData() const;

private:
    void setupUI();
    void populateFields(const CharacterData &data);

    QLineEdit *firstNameEdit;
    QLineEdit *lastNameEdit;
    QComboBox *houseComboBox;
    QComboBox *bloodStatusComboBox;
    QDateEdit *birthDateEdit;

    QPushButton *saveButton;
    QPushButton *cancelButton;

    int editingCharacterId = -1;
};
