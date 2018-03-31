#include "add_public_events_task.h"
#include <log.h>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

AddPublicEventsTask::AddPublicEventsTask(IWebSocketServer *pWebSocketServer, QString type, QString message){
	m_pWebSocketServer = pWebSocketServer;
	m_sType = type;
	m_sMessage = message;
	TAG = "AddPublicEventsTask";
};

AddPublicEventsTask::~AddPublicEventsTask(){
	
}

void AddPublicEventsTask::run(){
	Log::info(TAG, "message " + m_sMessage);
	QSqlDatabase db = *(m_pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO public_events(type,dt,message) VALUES(:type,NOW(),:message)");
	query.bindValue(":type", m_sType);
	query.bindValue(":message", m_sMessage);
	if(!query.exec()){
		Log::err(TAG, query.lastError().text());
	}
};
