QT += core widgets sql testlib gui

TEMPLATE = app
TARGET = test_runner
CONFIG += console qtest
CONFIG -= app_bundle

# Add the parent directory to the include path to find the main project's headers
INCLUDEPATH += ..

# Source files from the main project that are needed for the tests
SOURCES += ../src/CharacterService.cpp ../src/mainwindow.cpp ../src/DatabaseManager.cpp

# Test source files
SOURCES += \
    test_main.cpp \
    test_characterservice.cpp \
    test_mainwindow.cpp

# Test header files
HEADERS += \
    test_characterservice.h \
    ../src/CharacterService.h \
    ../src/mainwindow.h \
    test_mainwindow.h \
    ../src/DatabaseManager.h

