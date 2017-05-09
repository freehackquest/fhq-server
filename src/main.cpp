#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <QtCore>
#include <QFile>
#include <QString>
#include "websocketserver.h"
#include "exportapi.h"
#include "prepare_tmp_deb_package.h"

int main(int argc, char** argv) {
	QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("freehackquest-backend");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug", QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    
    QCommandLineOption exportApiOption(QStringList() << "e" << "export-api", QCoreApplication::translate("main", "Export API"));
    parser.addOption(exportApiOption);
    
    QCommandLineOption versionOption(QStringList() << "v" << "version", QCoreApplication::translate("main", "Version"));
    parser.addOption(versionOption);
    
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for freehackquest-backend [default: 1234]."),
            QCoreApplication::translate("main", "port"), QLatin1Literal("1234"));
    parser.addOption(portOption);
    parser.process(a);
    
    bool version = parser.isSet(versionOption);
    if(version){
		
		std::cout << PrepareTmpDebPackage::version().toStdString() << "\n";
		return 0;
	}
    
    bool exportApi = parser.isSet(exportApiOption);
    
    if(exportApi){
		ExportAPI exportApi;
		exportApi.exporttocommandline();
		return 0;
	}
    
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

	if(!QFile::exists("/etc/freehackquest-backend/conf.ini")){
		qDebug() << "Not found /etc/freehackquest-backend/conf.ini";
		return 0;
	}

	QThreadPool::globalInstance()->setMaxThreadCount(5);
    WebSocketServer *pServer = new WebSocketServer(port, debug);
    QObject::connect(pServer, &WebSocketServer::closed, &a, &QCoreApplication::quit);
    
    // TODO redesign to check config
    QSqlDatabase *db = pServer->database();
    if (!db->open()){
		return -1;
	}
	
	return a.exec();
}
