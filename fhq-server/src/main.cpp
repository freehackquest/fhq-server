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
#include <log.h>
#include <websocketserver.h>
#include <utils_prepare_deb_package.h>
#include <utils_create_config.h>
#include <utils_lxd.h>
#include <create_unit_tests.h>
#include <employees.h>
#include <employ_server_config.h>
#include <employ_server_info.h>
#include <employ_database.h>
#include <employ_settings.h>
#include <employ_images.h>
#include <model_help_args.h>
#include <iomanip>
#include <algorithm>
#include <utils_export_list_of_handlers.h>
#include <utils_export_client_library_python.h>

int main(int argc, char** argv) {
	QCoreApplication a(argc, argv);
	std::string TAG = "MAIN";
	Log::setdir("/var/log/fhq-server");

	HelpArgs helpArgs(argc, argv);
	helpArgs.addHelp(HelpArg("help", "-h", "This help"));
	helpArgs.addHelp(HelpArg("version", "-v", "Print version"));
	helpArgs.addHelp(HelpArg("run-unit-tests", "-rut", "Run unit tests"));
	helpArgs.addHelp(HelpArg("show-handlers", "-sh", "Show handlers"));
	helpArgs.addHelp(HelpArg("export-cli-library-python", "-eclp", "Export client library for python"));
	helpArgs.addHelp(HelpArg("show-employees", "-se", "Show employees"));
	helpArgs.addHelp(HelpArg("show-settings", "-ss", "Show settings"));
	helpArgs.addHelp(HelpArg("prepare-deb", "-pd", "Prepare Deb Package"));
	helpArgs.addHelp(HelpArg("check-server-config", "-csc", "Check server config"));
	helpArgs.addHelp(HelpArg("make-config-linux", "-mcl", "Create config file for Linux: /etc/fhq-server/fhq-server.conf"));
	helpArgs.addHelp(HelpArg("check-database-connection", "-cdc", "Check database conenction"));
	helpArgs.addHelp(HelpArg("manual-create-database", "-mcd", "Manual create database"));
    helpArgs.addHelp(HelpArg("manual-configure-lxd", "-mclxd", "Manual configure HTTPS connection with LXD. You need generated SSL cert and key in /etc/fhq-server/lxd"));
    helpArgs.addHelp(HelpArg("start", "-s", "Start server"));


	a.setApplicationName("fhq-server");
    a.setApplicationVersion(VERSION_STRING);

	if(argc > 3) {
		helpArgs.printHelp();
		return 0;
	} else if(helpArgs.has("help") || helpArgs.has("-h")) {
		helpArgs.printHelp();
		return 0;
	}else if(helpArgs.has("run-unit-tests") || helpArgs.has("-rut")){
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
	}else if(helpArgs.has("show-handlers") || helpArgs.has("-sh")){
	    UtilsExportListOfHandlers::print();
		return 0;
	}else if(helpArgs.has("export-cli-library-python") || helpArgs.has("-eclp")){
        UtilsExportClientLibraryPython::exportLib();
        return 0;
	}else if(helpArgs.has("show-employees") || helpArgs.has("-se")){
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
	}else if(helpArgs.has("version") || helpArgs.has("-v")){
		std::cout << QCoreApplication::applicationName().toStdString() << "-" << QCoreApplication::applicationVersion().toStdString() << "\n";
		return 0;
	}else if(helpArgs.has("--prepare-deb") || helpArgs.has("-pd")){
		UtilsPrepareDebPackage::prepare("","tmpdeb");
		return 0;
	}else if(helpArgs.has("make-config-linux") || helpArgs.has("-mcl")){
		UtilsCreateConfig::forLinux();
		return 0;
	}else if(helpArgs.has("check-server-config") || helpArgs.has("-csc")){
		std::cout << "\n * Check Server Config\n\n";
		EmployServerConfig *pConfig = new EmployServerConfig();
		if(!pConfig->init()){
			std::cout << "\n * FAIL\n\n";
		}else{
			std::cout << "\n * Success\n\n";
		}
		return 0;
	}else if(helpArgs.has("check-database-connection") || helpArgs.has("-cdc")){
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
	}else if(helpArgs.has("show-settings") || helpArgs.has("-ss")){
		Employees::init({});
		EmploySettings *pSettings = findEmploy<EmploySettings>();
		
		std::cout << "\n * Show settings\n\n";
		pSettings->printSettings();
		std::cout << "\n * Done\n\n";
		return 0;
	}else if(helpArgs.has("test-png")){
		std::cout << "\n * Test png\n\n";
		// Employees::init({});
		EmployImages *pImages = new EmployImages();
		// EmployImages *pImages = findEmploy<EmployImages>();
		pImages->doThumbnailImagePng("test.png", "test_100x100.png", 100, 100);
		pImages->doThumbnailImagePng("test_alpha.png", "test_alpha_100x100.png", 100, 100);
		std::cout << "\n * Done\n\n";
		return 0;
	}else if(helpArgs.has("manual-create-database") || helpArgs.has("-mcd")){
		std::cout << "\n * Manual create database\n\n";
		EmployServerConfig *pServerConfig = findEmploy<EmployServerConfig>();
		if(!pServerConfig->init()){
			std::cout << "\n * Failed on init server config\n\n";
			return -1;
		}
		EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
		
		// enter mysql root password
		char *pPassword=getpass("Enter MySQL root password: ");
		std::string sRootPassword(pPassword);
		std::string sError = "";
		if(!pDatabase->manualCreateDatabase(sRootPassword, sError)){
			std::cout << "\n * Failed: " << sError << "\n\n";
			return -1;
		}
		
		// init database
		if(!pDatabase->init()){
			std::cout << "\n * Failed on init database structure\n\n";
			return -1;
		}
		
		std::cout << "\n * Done\n\n";
		return 0;
    } else if(helpArgs.has("manual-configure-lxd") || helpArgs.has("-mclxd")){
        std::string sError;
        Employees::init({});
        if (UtilsLXDAuth::check_trust_certs(sError))
            std::cout << "\nGOOD HTTPS connection with LXD\n\n";
        else if (sError != ""){
            Log::err(TAG, "\nBAD HTTPS connection with LXD\n\n: " + sError);
            return -1;
        }
        else {
            char *pPassword=getpass("\nPlease enter your password for LXD:");
            std::string sPass(pPassword);
            if (UtilsLXDAuth::connect_with_lxd(sPass, sError))
                std::cout << "\nWell, you added your certificates to LXD.\n\n";
            else {
                std::cout << "\nBad, I can not make certificates trusted. Maybe the password was wrong? \nError:" + sError + "\n\n";
                return -1;
            }
        }
        return 0;
	}else if(helpArgs.has("start") || helpArgs.has("-s")){
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

	helpArgs.printHelp();
	return 0;
}
