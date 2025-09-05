QT       += core gui widgets sql
TARGET = Sruighlea
TEMPLATE = app
SOURCES += \
    main.cpp \
    src/CharacterService.cpp \
    src/mainwindow.cpp \
    src/DatabaseManager.cpp \
    src/addcharacterdialog.cpp

HEADERS += \
    src/CharacterService.h \
    src/mainwindow.h \
    src/DatabaseManager.h \
    src/addcharacterdialog.h
