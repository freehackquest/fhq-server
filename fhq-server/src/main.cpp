// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <signal.h>
#include <iostream>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/time.h>
// #include <unistd.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <syslog.h>
#include <QtCore>
#include <QFile>
#include <QString>
#include <log.h>
#include <websocketserver.h>
#include <utils_prepare_deb_package.h>
#include <utils_create_config.h>
#include <create_unit_tests.h>
#include <employees.h>
#include <employ_server_info.h>
#include <employ_database.h>

int main(int argc, char** argv) {
	QCoreApplication a(argc, argv);
	QString TAG = "main";
	Log::setdir("/var/log/freehackquest-backend");
	
	a.setApplicationName("freehackquest-backend");
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

	bool bRunUnitTests = parser.isSet(optRunUnitTests);
    if(bRunUnitTests){
		QMap<QString, IUnitTest *> mapUnitTests;
		create_unit_tests(mapUnitTests);
		foreach( QString name, mapUnitTests.keys()){
			IUnitTest *pUnitTest = mapUnitTests.value(name);
			Log::info(TAG,  "Run test  " + name);
			if(pUnitTest->run()){
				Log::info(TAG,  "Test passed");
			}else{
				Log::err(TAG,  "Test failed");
			}
		}
		return 0;
	}


	bool bPrepareDebPackage = parser.isSet(prepareDebOption);
    if(bPrepareDebPackage){
		UtilsPrepareDebPackage::prepare("","tmpdeb");
		return 0;
	}

	bool bCreateConfigLinux = parser.isSet(optCreateConfigLinux);
	if(bCreateConfigLinux){
		UtilsCreateConfig::forLinux();
		return 0;
	};

	QThreadPool::globalInstance()->setMaxThreadCount(5);
    WebSocketServer *pServer = new WebSocketServer();
    if(pServer->isFailed()){
        Log::err(TAG, "Could not start server");
        return -1;
    }

    QObject::connect(pServer, &WebSocketServer::closed, &a, &QCoreApplication::quit);
    
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    // TODO redesign to check config
    QSqlDatabase *db = pDatabase->database();
    if (!db->open()){
		return -1;
	}
	
	return a.exec();
}
