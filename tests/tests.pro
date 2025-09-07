QT += core widgets sql testlib gui

TEMPLATE = app
TARGET = test_runner
CONFIG += console qtest
CONFIG -= app_bundle

# Add the parent directory to the include path and source path
VPATH = ../src
INCLUDEPATH += ../src

# Source files from the main project that are needed for the tests
SOURCES += CharacterService.cpp mainwindow.cpp DatabaseManager.cpp addcharacterdialog.cpp Logger.cpp

# Test source files
SOURCES += \
    test_main.cpp \
    test_characterservice.cpp \
    test_mainwindow.cpp \
    test_logger.cpp

# Test header files
HEADERS += \
    test_characterservice.h \
    CharacterService.h \
    mainwindow.h \
    test_mainwindow.h \
    DatabaseManager.h \
    addcharacterdialog.h \
    Logger.h \
    test_logger.h
