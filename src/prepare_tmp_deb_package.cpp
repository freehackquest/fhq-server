#include "prepare_tmp_deb_package.h"
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QSysInfo>

QString PrepareTmpDebPackage::version(){
	return "0.1.15";
};

QString PrepareTmpDebPackage::name(){
	return "freehackquest-backend";
}

void PrepareTmpDebPackage::prepare(QString repo, QString tmpdeb){
	std::cout << "Distribution: " + QSysInfo::productType().toStdString() + "\n";
	std::cout << "Distribution version: " + QSysInfo::productVersion().toStdString() + "\n";

	QFile file(tmpdeb + "/DEBIAN/control");
	if(file.exists()) file.remove();
	if(file.open(QIODevice::ReadWrite)){
		QTextStream stream(&file);
		stream << "Source: " << PrepareTmpDebPackage::name() << endl;
		stream << "Section: misc" << endl;
		stream << "Priority: optional" << endl;
		// TODO redesign in static method
		stream << "Maintainer: Evgenii Sopov <mrseakg@gmail.com>" << endl;
		// TODO different for ubuntu (default-mysql-server) and for debian (mysql-server)
		QStringList depends;
		if(QSysInfo::productType() == "ubuntu"){
			depends << "default-mysql-server";
		}else if(QSysInfo::productType() == "debian"){
			depends << "mysql-server";
		}else{
			depends << "mysql-server";
		}
		depends << "libqt5websockets5" << "libqt5network5" << "libqt5sql5" << "libqt5sql5-mysql" << "libqt5core5a";
		depends << "libc6" << "libstdc++6" << "libgcc1" << "zlib1g" << "libicu52" << "libglib2.0-0" << "libpcre3";

		stream << "Depends: " << depends.join(", ") << endl;
		
		// Version
		stream << "Version: " << PrepareTmpDebPackage::version() << endl;
		std::cout << "Version: " + PrepareTmpDebPackage::version().toStdString() + "\n";
		
		 // TODO calculate correct installed size
		stream << "Installed-Size: 1024" << endl;
		stream << "Homepage: https://github.com/freehackquest/backend" << endl;
		
		// Package
		stream << "Package: " << PrepareTmpDebPackage::name() << endl;
		std::cout << "Package: " + PrepareTmpDebPackage::name().toStdString() + "\n";
		
		// Architecture
		if(QSysInfo::currentCpuArchitecture() == "x86_64"){
			stream << "Architecture: amd64" << endl;
			std::cout << "Architecture: amd64\n";
		} else if(QSysInfo::currentCpuArchitecture() == "i386"){
			stream << "Architecture: i386" << endl;
			std::cout << "Architecture: i386\n";
		}else{
			stream << "Architecture: all" << endl;
			std::cout << "Architecture: all\n";
		}

		stream << "Description: Backend for FreeHackQuest" << endl;
		stream << "  This is an open source platform for competitions in computer security." << endl;
	}
}
