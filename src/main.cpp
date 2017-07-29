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
#include <log.h>
#include <websocketserver.h>
#include "prepare_tmp_deb_package.h"

int main(int argc, char** argv) {
	QCoreApplication a(argc, argv);
	QString TAG = "main";
	Log::setdir("/var/log/freehackquest-backend");
	
	a.setApplicationName("freehackquest-backend");
    a.setApplicationVersion(VERSION_STRING);
    
    
    QCommandLineParser parser;
    parser.setApplicationDescription("freehackquest-backend");

    
    parser.addHelpOption();
    
    QCommandLineOption versionOption(QStringList() << "v" << "version", QCoreApplication::translate("main", "Version"));
    parser.addOption(versionOption);
    
    QCommandLineOption prepareDebOption(QStringList() << "pd" << "prepare-deb", QCoreApplication::translate("main", "Prepare Deb Package"));
    parser.addOption(prepareDebOption);

    parser.process(a);
    std::cout << QCoreApplication::applicationName().toStdString() << "-" << QCoreApplication::applicationVersion().toStdString() << "\n";
    
    bool version = parser.isSet(versionOption);
    if(version){
		return 0;
	}

	bool prepare_deb = parser.isSet(prepareDebOption);
    if(prepare_deb){
		PrepareTmpDebPackage::prepare("","tmpdeb");
		return 0;
	}

	if(!QFile::exists("/etc/freehackquest-backend/conf.ini")){
		Log::err(TAG, "Not found /etc/freehackquest-backend/conf.ini");
		return 0;
	}

	QThreadPool::globalInstance()->setMaxThreadCount(5);
    WebSocketServer *pServer = new WebSocketServer();
    QObject::connect(pServer, &WebSocketServer::closed, &a, &QCoreApplication::quit);
    
    // TODO redesign to check config
    QSqlDatabase *db = pServer->database();
    if (!db->open()){
		return -1;
	}
	
	return a.exec();
}
