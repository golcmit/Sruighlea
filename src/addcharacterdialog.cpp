#include "addcharacterdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QDialogButtonBox>

AddCharacterDialog::AddCharacterDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Add New Character");
    setupUI();
}

void AddCharacterDialog::setupUI()
{
    QFormLayout *formLayout = new QFormLayout();

    firstNameEdit = new QLineEdit(this);
    lastNameEdit = new QLineEdit(this);
    houseComboBox = new QComboBox(this);
    bloodStatusComboBox = new QComboBox(this);
    birthDateEdit = new QDateEdit(this);
    birthDateEdit->setDate(QDate::currentDate());
    birthDateEdit->setCalendarPopup(true);

    // Populate combo boxes (ideally this would come from the database)
    houseComboBox->addItems({"gryffindor", "hufflepuff", "ravenclaw", "slytherin"});
    bloodStatusComboBox->addItems({"pure", "half", "muggleborn"});

    formLayout->addRow("First Name:", firstNameEdit);
    formLayout->addRow("Last Name:", lastNameEdit);
    formLayout->addRow("House:", houseComboBox);
    formLayout->addRow("Blood Status:", bloodStatusComboBox);
    formLayout->addRow("Birth Date:", birthDateEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    saveButton = buttonBox->button(QDialogButtonBox::Save);
    cancelButton = buttonBox->button(QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
}

CharacterData AddCharacterDialog::getCharacterData() const
{
    CharacterData data;
    data.firstName = firstNameEdit->text();
    data.lastName = lastNameEdit->text();
    data.house = houseComboBox->currentText();
    data.bloodStatus = bloodStatusComboBox->currentText();
    data.birthDate = birthDateEdit->date();
    return data;
}
