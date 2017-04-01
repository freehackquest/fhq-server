#include "exportapi.h"
#include <QMap>
#include "cmd_handlers/create_cmd_handlers.h"

ExportAPI::ExportAPI(){
};

void ExportAPI::exporttocommandline(){
	qDebug().nospace().noquote() << "# FreeHackQuest API (WebSocket port 1234)\n";
	QMap<QString, ICmdHandler *> pHandlers;
	create_cmd_handlers(pHandlers);
	foreach( QString key, pHandlers.keys()){
		ICmdHandler *pHandler = pHandlers.value(key);
		qDebug().nospace().noquote() << "## Command `" << pHandler->cmd() << "`\n";
		
		qDebug().nospace().noquote() << pHandler->short_description() << "\n";
		
		qDebug().nospace().noquote() << "### Description\n";
		qDebug().nospace().noquote() << pHandler->description() << "\n";
				
		qDebug().nospace().noquote() << "### Access\n";
		qDebug().nospace().noquote() << " * " << (pHandler->accessUnauthorized() ? "Allowed" : "Denied") << " access unauthorized requests";
		qDebug().nospace().noquote() << " * " << (pHandler->accessUser() ? "Allowed" : "Denied") << " access user requests";
		qDebug().nospace().noquote() << " * " << (pHandler->accessTester() ? "Allowed" : "Denied") << " access tester requests";
		qDebug().nospace().noquote() << " * " << (pHandler->accessAdmin() ? "Allowed" : "Denied") << " access admin requests";
		qDebug().nospace().noquote() << "\n";
		
		QStringList errors = pHandler->errors();
		if(errors.size() > 0){
			qDebug().nospace().noquote() << "### Errors\n";
			for(int i = 0; i < errors.size(); i++){
				qDebug().nospace().noquote() << " * " << errors.at(i);
			}
			qDebug().nospace().noquote() << "\n";
		}
	}
};
