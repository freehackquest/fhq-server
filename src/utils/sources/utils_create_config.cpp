#include <utils_create_config.h>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QSysInfo>
#include <QtCore>

void UtilsCreateConfig::forLinux(){
	std::cout << "UtilsCreateConfig::forLinux begin\n";
	std::cout << "Distribution: " + QSysInfo::productType().toStdString() + "\n";
	std::cout << "Distribution version: " + QSysInfo::productVersion().toStdString() + "\n";
	
	// QDir dirEtc("/etc");
	// if(file.exists()) file.remove();
	
};
