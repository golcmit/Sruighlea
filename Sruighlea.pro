QT       += core gui widgets sql
TARGET = Sruighlea
TEMPLATE = app
SOURCES += \
    main.cpp \
    src/Services/CharacterService.cpp \
    src/UI/mainwindow.cpp \
    src/Core/DatabaseManager.cpp \
    src/UI/addcharacterdialog.cpp \
    src/Core/Logger.cpp

HEADERS += \
    src/Services/CharacterService.h \
    src/UI/mainwindow.h \
    src/Core/DatabaseManager.h \
    src/UI/addcharacterdialog.h \
    src/Core/Logger.h \
    src/Core/SqlQueries.h \
    src/Models/AcademicRecord.h \
    src/Models/AddressHistory.h \
    src/Models/Character.h \
    src/Models/GroupMembership.h \
    src/Models/OccupationRecord.h \
    src/Models/Relationship.h \
