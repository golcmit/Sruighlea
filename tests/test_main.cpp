#include "test_characterservice.h"
#include <QApplication>
#include <QTest>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int status = 0;

    {
        TestCharacterService tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    // Add other test classes here in the future

    return status;
}
