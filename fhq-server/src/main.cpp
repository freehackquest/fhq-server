#include <iostream>
#include <QtCore>
#include <QFile>
#include <QString>

int main(int argc, char** argv) {
	QCoreApplication a(argc, argv);
	QString TAG = "main";
	
	a.setApplicationName("fhq-server");
    a.setApplicationVersion(VERSION_STRING);
    
    QCommandLineParser parser;
    parser.setApplicationDescription("freehackquest-backend");
    
    parser.addHelpOption();
    
    QCommandLineOption versionOption(QStringList() << "ver" << "version", QCoreApplication::translate("main", "Version"));
    parser.addOption(versionOption);
    
    QCommandLineOption prepareDebOption(QStringList() << "pd" << "prepare-deb", QCoreApplication::translate("main", "Prepare Deb Package"));
    parser.addOption(prepareDebOption);
    
    QCommandLineOption optRunUnitTests(QStringList() << "rut" << "run-unit-tests", QCoreApplication::translate("main", "Run unit tests"));
    parser.addOption(optRunUnitTests);

    QCommandLineOption optTest(QStringList() << "t" << "test", QCoreApplication::translate("main", "Run test"));
    parser.addOption(optTest);

    QCommandLineOption optCreateConfigLinux(QStringList() << "cc-lin" << "create-config-linux", QCoreApplication::translate("main", "Create config file for Linux: /etc/freehackquest-backend/conf.ini"));
    parser.addOption(optCreateConfigLinux);

    parser.process(a);
    std::cout << QCoreApplication::applicationName().toStdString() << "-" << QCoreApplication::applicationVersion().toStdString() << "\n";
    
    bool version = parser.isSet(versionOption);
    if(version){
		return 0;
	}


    return 0;
}
