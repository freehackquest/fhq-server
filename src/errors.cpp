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
		qDebug().nospace().noquote() << dir.absolutePath() << " did not found";
		qDebug().nospace().noquote() << errorInfo;
		return;
	}
	QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	if(!dir.exists(date) && !dir.mkdir(date)){
		qDebug().nospace().noquote() << dir.absolutePath() << " could not create dir " << date;
		qDebug().nospace().noquote() << errorInfo;
		return;
	}
	dir.cd(date);
	
	QString time = QDateTime::currentDateTime().toString("HHmmss.zzz");
    QFile errorf(dir.absolutePath() + '/' + time);
    errorf.open(QIODevice::WriteOnly | QIODevice::Text);

    if(!errorf.isOpen()){
        qDebug().nospace().noquote() << dir.absolutePath() + '/' + time << " could not openfile ";
		qDebug().nospace().noquote() << errorInfo;
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
	Error error(1011, "Parameter 'userid' must be integer");
	return error;
}

Error Errors::UserIDMustBeNotZero(){
	Error error(1012, "Parameter 'userid' must be not zero");
	return error;
}
