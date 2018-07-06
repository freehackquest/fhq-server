#include <errors.h>

#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QDir>

// ---------------------------------------------------------------------

Error Errors::NotAuthorizedRequest(){
	Error error(401, "Not Authorized Request");
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
	Error error(404, "Not implemented yet");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::NotImplementedYet(const QString &comments)
{
    Error error(404, "Not implemented yet: " + comments);
    return error;
}

// ---------------------------------------------------------------------

Error Errors::NoneError(){
	Error error(404, "none");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::NotFound(QString what){
	Error error(404, "Not found " + what);
	return error;
}

// ---------------------------------------------------------------------

Error Errors::LostDatabaseConnection(){
	Error error(500, "Lost Database Connection");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::QuestIDMustBeNotZero(){
	Error error(400, "Parameter 'questid' must be not zero");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::HintIDMustBeNotZero(){
	Error error(400, "Parameter 'hintid' must be not zero");
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

Error Errors::ParamMustBeEmail(QString param_name){
    Error error(400, "Parameter '" + param_name + "' must be email type");
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
		
Error Errors::InvalidToken(){
	Error error(401, "Invalid token");
	return error;
}

// ---------------------------------------------------------------------

Error Errors::DatabaseError(QString text){
	Error error(500, "Database error. " + text);
	return error;
}

// ---------------------------------------------------------------------
