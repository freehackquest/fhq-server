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
#include <employ_server_config.h>
#include <employ_server_info.h>
#include <employ_database.h>
#include <employ_settings.h>
#include <employ_images.h>

void print_help(std::vector<std::string> &vArgs){
	std::cout
		<< "Usage: " << vArgs.at(0) << "   [PARAMS]\n"
		<< "\t --help, -h                             This help \n"
		<< "\t --version, -v                          Print version \n"
		<< "\t --run-unit-tests, -rut                 Run unit tests\n"
		<< "\t --show-handlers, -sh                   Show handlers\n"
		<< "\t --show-employees, -se                  Show employees\n"
		<< "\t --show-settings, -ss                   Show settings\n"
		<< "\t --prepare-deb, -pd                     Prepare Deb Package\n"
		<< "\t --check-server-config, -csc            Check server config\n"
		<< "\t --create-config-linux, -ccl            Create config file for Linux: /etc/fhq-server/conf.ini \n"
		<< "\t --check-database-connection, -cdc      Check database conenction\n"
		<< "\t --server, -s                           Start server\n"
		<< "\n";
}

bool hasArgs(std::vector<std::string> &vArgs, std::string s){
	return std::find(vArgs.begin(), vArgs.end(), s) != vArgs.end();
}

int main(int argc, char** argv) {
	QCoreApplication a(argc, argv);
	std::string TAG = "MAIN";
	Log::setdir("/var/log/fhq-server");
	
	a.setApplicationName("fhq-server");
    a.setApplicationVersion(VERSION_STRING);
    
    std::vector<std::string> vArgs;
    for(int i = 0; i < argc; i++){
		vArgs.push_back(std::string(argv[i]));
	}

	if(hasArgs(vArgs, "--help") || hasArgs(vArgs, "-h")) {
		print_help(vArgs);
		return 0;
	}else if(hasArgs(vArgs, "--run-unit-tests") || hasArgs(vArgs, "-rut")){
		QMap<QString, IUnitTest *> mapUnitTests;
		create_unit_tests(mapUnitTests);
		foreach( QString name, mapUnitTests.keys()){
			IUnitTest *pUnitTest = mapUnitTests.value(name);
			Log::info(TAG,  "Run test  " + name.toStdString());
			if(pUnitTest->run()){
				Log::info(TAG,  "Test passed");
			}else{
				Log::err(TAG,  "Test failed");
			}
		}
		return 0;
	}else if(hasArgs(vArgs, "--show-handlers") || hasArgs(vArgs, "-sh")){
		std::cout << "\n\n * CmdHandlers (" << g_pCmdHandlers->size() << "):\n";
		std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
		for (; it!=g_pCmdHandlers->end(); ++it){
			std::string sCmd = it->first;
			CmdHandlerBase* pCmdHandlerBase = it->second;
			std::cout << " |--- * " << sCmd << "\n";
		}
		std::cout << "\n\n";
		return 0;
	}else if(hasArgs(vArgs, "--show-employees") || hasArgs(vArgs, "-se")){
		std::cout << " * Employees (" << g_pEmployees->size() << "):\n";
		std::map<std::string, EmployBase*>::iterator it = g_pEmployees->begin();
		for (; it!=g_pEmployees->end(); ++it){
			std::string sEmployName = it->first;
			EmployBase* pEmployBase = it->second;
			std::cout << " |--- * " << sEmployName << "\n";
			if(pEmployBase->loadAfter().size() > 0){
				for(int i = 0; i < pEmployBase->loadAfter().size(); i++){
					std::cout << " |    +--- * after: " << pEmployBase->loadAfter().at(i) << "\n";
				}
			}
			std::cout << " |  \n";
		}
		return 0;
	}else if(hasArgs(vArgs, "--version") || hasArgs(vArgs, "-v")){
		std::cout << QCoreApplication::applicationName().toStdString() << "-" << QCoreApplication::applicationVersion().toStdString() << "\n";
		return 0;
	}else if(hasArgs(vArgs, "--prepare-deb") || hasArgs(vArgs, "-pd")){
		UtilsPrepareDebPackage::prepare("","tmpdeb");
		return 0;
	}else if(hasArgs(vArgs, "--create-config-linux") || hasArgs(vArgs, "-ccl")){
		UtilsCreateConfig::forLinux();
		return 0;
	}else if(hasArgs(vArgs, "--check-server-config") || hasArgs(vArgs, "-csc")){
		std::cout << "\n * Check Server Config\n\n";
		EmployServerConfig *pConfig = new EmployServerConfig();
		if(!pConfig->init()){
			std::cout << "\n * FAIL\n\n";
		}else{
			std::cout << "\n * Success\n\n";
		}
		return 0;
	}else if(hasArgs(vArgs, "--check-database-connection") || hasArgs(vArgs, "-cdc")){
		std::cout << "\n * Check Database Connection\n\n";
		Employees::init({});
		EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
		QSqlDatabase *db = pDatabase->database();
		if (!db->open()){
			Log::err(TAG, "Could not connect to database, please check config");
			return -1;
		}
		std::cout << "\n * Success\n\n";
		return 0;
	}else if(hasArgs(vArgs, "--show-settings") || hasArgs(vArgs, "-ss")){
		std::cout << "\n * Show settings\n\n";
		Employees::init({});
		EmploySettings *pSettings = findEmploy<EmploySettings>();
		pSettings->printSettings();
		std::cout << "\n * Done\n\n";
		return 0;
	}else if(hasArgs(vArgs, "--test-png")){
		std::cout << "\n * Test png\n\n";
		// Employees::init({});
		EmployImages *pImages = new EmployImages();
		// EmployImages *pImages = findEmploy<EmployImages>();
		pImages->doThumbnailImagePng("test.png", "test_100x100.png", 100, 100);
		pImages->doThumbnailImagePng("test_alpha.png", "test_alpha_100x100.png", 100, 100);
		std::cout << "\n * Done\n\n";
		return 0;
	}else if(hasArgs(vArgs, "--server") || hasArgs(vArgs, "-s")){
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

	print_help(vArgs);
	return 0;
}
