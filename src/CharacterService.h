#pragma once

#include <QString>
#include <QDate>
#include <QObject>

// A simple struct to hold character details
struct CharacterData {
    int id = -1;
    QString firstName;
    QString lastName;
    QDate birthDate;
    QString house;
    QString bloodStatus;
    // Add other fields as needed in the future

    bool isValid() const { return id != -1; }
};

class CharacterService : public QObject
{
    Q_OBJECT
public:
    explicit CharacterService(QObject *parent = nullptr);

    CharacterData getCharacterDetails(int characterId);
};
