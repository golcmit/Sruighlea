QT += core widgets sql testlib

TEMPLATE = app
TARGET = test_runner
CONFIG += console qtest
CONFIG -= app_bundle

# Add the parent directory to the include path to find the main project's headers
INCLUDEPATH += ..

# Source files from the main project that are needed for the tests
# We will add CharacterService.cpp here once it's created.
SOURCES += ../src/CharacterService.cpp

# Test source files
SOURCES += \
    test_main.cpp \
    test_characterservice.cpp

# Test header files
HEADERS += \
    test_characterservice.h \
    ../src/CharacterService.h

