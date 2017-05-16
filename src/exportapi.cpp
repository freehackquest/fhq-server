#include "exportapi.h"
#include <QMap>
#include "cmd/create_cmd_handlers.h"

ExportAPI::ExportAPI(){
};

void ExportAPI::exporttocommandline(){
	qDebug().nospace() << "# FreeHackQuest API (WebSocket port 1234)\n";
	QMap<QString, ICmdHandler *> pHandlers;
	create_cmd_handlers(pHandlers);
	foreach( QString key, pHandlers.keys()){
		ICmdHandler *pHandler = pHandlers.value(key);
		qDebug().nospace() << "## Command `" << pHandler->cmd() << "`\n";
		
		qDebug().nospace() << pHandler->short_description() << "\n";
		
		qDebug().nospace() << "### Description\n";
		qDebug().nospace() << pHandler->description() << "\n";
				
		qDebug().nospace() << "### Access\n";
		qDebug().nospace() << " * " << (pHandler->accessUnauthorized() ? "Allowed" : "Denied") << " access unauthorized requests";
		qDebug().nospace() << " * " << (pHandler->accessUser() ? "Allowed" : "Denied") << " access user requests";
		qDebug().nospace() << " * " << (pHandler->accessTester() ? "Allowed" : "Denied") << " access tester requests";
		qDebug().nospace() << " * " << (pHandler->accessAdmin() ? "Allowed" : "Denied") << " access admin requests";
		qDebug().nospace() << "\n";
		
		QStringList errors = pHandler->errors();
		if(errors.size() > 0){
			qDebug().nospace() << "### Errors\n";
			for(int i = 0; i < errors.size(); i++){
				qDebug().nospace() << " * " << errors.at(i);
			}
			qDebug().nospace() << "\n";
		}
	}
};
