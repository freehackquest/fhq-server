#include "errors.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDateTime>
#include <QDir>

// use this function for problems with database connection
void Errors::WriteServerError(QString errorInfo){
	QDir dir("/var/log/freehackquestd/errors");
	if(!dir.exists()){
		qDebug().nospace() << dir.absolutePath() << " did not found";
		qDebug().nospace() << errorInfo;
		return;
	}
	QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	if(!dir.exists(date) && !dir.mkdir(date)){
		qDebug().nospace() << dir.absolutePath() << " could not create dir " << date;
		qDebug().nospace() << errorInfo;
		return;
	}
	dir.cd(date);
	
	QString time = QDateTime::currentDateTime().toString("HHmmss.zzz");
    QFile errorf(dir.absolutePath() + '/' + time);
    errorf.open(QIODevice::WriteOnly | QIODevice::Text);

    if(!errorf.isOpen()){
        qDebug().nospace() << dir.absolutePath() + '/' + time << " could not openfile ";
		qDebug().nospace() << errorInfo;
		return;
    }
    QTextStream outStream(&errorf);
    outStream << errorInfo.toUtf8();
    errorf.close();
}


Error Errors::NotAuthorizedRequest(){
	Error error(1001, "Not Authorized Request");
	return error;
}

Error Errors::AllowedOnlyForAdmin(){
	Error error(1002, "Allowed only for admin");
	return error;
}

Error Errors::NotImplementedYet(){
	Error error(1003, "Not implemented yet");
	return error;
}

Error Errors::NotFoundUserByUUID(QString uuid){
	Error error(1004, "Not found user by uuid " + uuid);
	return error;
}

Error Errors::NotFoundUUIDField(){
	Error error(1005, "Not found uuid field");
	return error;
}

Error Errors::LostDatabaseConnection(){
	Error error(1006, "Lost Database Connection");
	return error;
}

Error Errors::QuestIDMustBeInteger(){
	Error error(1007, "Parameter 'questid' must be integer");
	return error;
}

Error Errors::QuestIDMustBeNotZero(){
	Error error(1008, "Parameter 'questid' must be not zero");
	return error;
}

Error Errors::HintIDMustBeInteger(){
	Error error(1009, "Parameter 'hintid' must be integer");
	return error;
}

Error Errors::HintIDMustBeNotZero(){
	Error error(1010, "Parameter 'hintid' must be not zero");
	return error;
}

Error Errors::UserIDMustBeInteger(){
	Error error(400, "Parameter 'userid' must be integer");
	return error;
}

Error Errors::UserIDMustBeNotZero(){
	Error error(400, "Parameter 'userid' must be not zero");
	return error;
}

Error Errors::ParamTableMustBeActiveOrBackup(){
	Error error(400, "Parameter 'table' must be 'active' or 'backup'");
	return error;
}

Error Errors::OnPageCouldNotBeMoreThen50(){
	Error error(400, "Parameter 'onpage' could not be more then 50");
	return error;
}

Error Errors::GameIDMustBeInteger(){
	Error error(400, "Parameter 'gameid' must be integer");
	return error;
}

Error Errors::OnPageMustBeInteger(){
	Error error(400, "Parameter 'onpage' must be integer");
	return error;
}

Error Errors::PageMustBeInteger(){
	Error error(400, "Parameter 'page' must be integer");
	return error;
}

Error Errors::InternalServerError(){
	Error error(500, "Internal Server Error");
	return error;
}

Error Errors::EventIdMustBeInteger(){
	Error error(400, "Parameter 'eventid' must be integer");
	return error;
}

Error Errors::EventIdExpected(){
	Error error(400, "Parameter 'eventid' expected");
	return error;
}

Error Errors::EventNotFound(){
	Error error(404, "Event not found");
	return error;
}

Error Errors::EventTypeExpected(){
	Error error(400, "Parameter 'type' expected");
	return error;
}

Error Errors::EventMessageExpected(){
	Error error(400, "Parameter 'message' expected");
	return error;
}

Error Errors::EventTypeValueExpected(){
	Error error(400, "Parameter 'type' can be has only one from array: [info, users, games, quests, warning]");
	return error;
}
