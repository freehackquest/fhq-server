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

// ---------------------------------------------------------------------

Error Errors::NotAuthorizedRequest(){
	Error error(1001, "Not Authorized Request");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::AccessDenyForUser(){
	Error error(400, "Access deny for user");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::AccessDenyForTester(){
	Error error(400, "Access deny for tester");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::AccessDenyForAdmin(){
	Error error(400, "Access deny for admin");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::NotImplementedYet(){
	Error error(1003, "Not implemented yet");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::NotFoundUserByUUID(QString uuid){
	Error error(1004, "Not found user by uuid " + uuid);
	return error;
}

// ---------------------------------------------------------------------

Error Errors::NoneError(){
	Error error(0, "none");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::LostDatabaseConnection(){
	Error error(1006, "Lost Database Connection");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::QuestIDMustBeNotZero(){
	Error error(1008, "Parameter 'questid' must be not zero");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::HintIDMustBeNotZero(){
	Error error(1010, "Parameter 'hintid' must be not zero");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::UserIDMustBeNotZero(){
	Error error(400, "Parameter 'userid' must be not zero");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::ParamTableMustBeActiveOrBackup(){
	Error error(400, "Parameter 'table' must be 'active' or 'backup'");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::OnPageCouldNotBeMoreThen50(){
	Error error(400, "Parameter 'onpage' could not be more then 50");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::InternalServerError(){
	Error error(500, "Internal Server Error");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::EventNotFound(){
	Error error(404, "Event not found");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::ParamExpected(QString param_name){
	Error error(400, "Parameter '" + param_name + "' expected");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::ParamMustBeInteger(QString param_name){
	Error error(400, "Parameter '" + param_name + "' must be integer");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::ParamExpectedValueOneFrom(QString param_name, QStringList eList){
	Error error(400, "Parameter '" + param_name + "' expected values one from [" + eList.join(",") + "]");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::ParamExpectedUUID(QString param_name){
	Error error(400, "Parameter '" + param_name + "' expected uuid regexp: /^[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}$/i");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::ParamMustBeLessThen(QString param_name, int maxval){
	Error error(400, "Parameter '" + param_name + "' must be less then " + QString::number(maxval));
	return error;
}

// ---------------------------------------------------------------------

Error Errors::ParamMustBeMoreThen(QString param_name, int minval){
	Error error(400, "Parameter '" + param_name + "' must be more then " + QString::number(minval));
	return error;
}

// ---------------------------------------------------------------------
		
