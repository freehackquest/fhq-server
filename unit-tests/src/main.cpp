#include <iostream>
#include <string>
#include <unistd.h> // getpass

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <signal.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/time.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <syslog.h>
#include <QtCore>
#include <QFile>
#include <QString>
#include <logger.h>
#include <unit_tests.h>
#include <iomanip>
#include <algorithm>

int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);
    std::string TAG = "MAIN";
    Log::setdir("./");

    if (!UnitTests::runUnitTests()) {
        return -1;
    }
    return 0;
}
