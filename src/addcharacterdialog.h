#pragma once

#include <QDialog>
#include "CharacterService.h" // For CharacterData

class QLineEdit;
class QComboBox;
class QDateEdit;
class QPushButton;

class AddCharacterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCharacterDialog(QWidget *parent = nullptr);
    CharacterData getCharacterData() const;

private:
    void setupUI();

    QLineEdit *firstNameEdit;
    QLineEdit *lastNameEdit;
    QComboBox *houseComboBox;
    QComboBox *bloodStatusComboBox;
    QDateEdit *birthDateEdit;

    QPushButton *saveButton;
    QPushButton *cancelButton;
};
