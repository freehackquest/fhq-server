#include "../headers/cmd_answerlist_handler.h"
#include <QJsonArray>

QString CmdAnswerListHandler::cmd(){
	return "answerlist";
}

bool CmdAnswerListHandler::accessUnauthorized(){
	return false;
}

bool CmdAnswerListHandler::accessUser(){
	return false;
}

bool CmdAnswerListHandler::accessTester(){
	return false;
}

bool CmdAnswerListHandler::accessAdmin(){
	return true;
}

QString CmdAnswerListHandler::short_description(){
	return "some short description";
}

QString CmdAnswerListHandler::description(){
	return "some description";
}

QStringList CmdAnswerListHandler::errors(){
	QStringList	list;
	return list;
}

void CmdAnswerListHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());

	int nPage = 0;
	if(obj.contains("page")){
		QJsonValueRef vPage = obj["page"];
		if(!vPage.isDouble()){
			pWebSocketServer->sendMessageError(pClient, cmd(), Errors::PageMustBeInteger());
			return;
		}
		nPage = vPage.toInt();
	}
	jsonData["page"] = nPage;
	
	int nOnPage = 10;
	if(obj.contains("onpage")){
		QJsonValueRef vOnPage = obj["onpage"];
		if(!vOnPage.isDouble()){
			pWebSocketServer->sendMessageError(pClient, cmd(), Errors::OnPageMustBeInteger());
			return;
		}
		nOnPage = vOnPage.toInt();
	}
	if(nOnPage > 50){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::OnPageCouldNotBeMoreThen50());
		return;
	}
	jsonData["onpage"] = nOnPage;

	QStringList filters;
	QMap<QString,QString> filter_values;

	if(obj.contains("userid")){
		QJsonValueRef vUserID = obj["userid"];
		if(!vUserID.isDouble()){
			pWebSocketServer->sendMessageError(pClient, cmd(), Errors::UserIDMustBeInteger());
			return;
		}
		int userid = vUserID.toInt();
		filters << "(u.id = :userid)";
		filter_values[":userid"] = userid;
	}
	
	if(obj.contains("user")){
		QString user = obj["user"].toString().trimmed();
		filters << "(u.email like :email OR u.nick like :nick)";
		filter_values[":email"] = "%" + user + "%";
		filter_values[":nick"] = "%" + user + "%";
	}

	if(obj.contains("questid")){
		QJsonValueRef vQuestID = obj["questid"];
		if(!vQuestID.isDouble()){
			pWebSocketServer->sendMessageError(pClient, cmd(), Errors::QuestIDMustBeInteger());
			return;
		}
		int questid = vQuestID.toInt();
		filters << "(q.idquest = :questid)";
		filter_values[":questid"] = questid;
	}
	
	if(obj.contains("questname")){
		QString questname = obj["questname"].toString().trimmed();
		if(questname != ""){
			filters << "(q.name LIKE :questname)";
			filter_values[":questname"] = "%" + questname + "%";
		}
	}
	
	if(obj.contains("questsubject")){
		QString questsubject = obj["questsubject"].toString().trimmed();
		if(questsubject != ""){
			filters << "(q.subject = :questsubject)";
			filter_values[":questsubject"] = questsubject;
		}
	}

	if(obj.contains("passed")){
		QString passed = obj["passed"].toString().trimmed();
		if(passed != ""){
			filters << "(uqa.passed = :passed)";
			filter_values[":passed"] = passed;
		}
	}

	QString where = filters.join(" AND "); 
	if(where.length() > 0){
		where = "WHERE " + where;
	}
	
	// count quests
	QSqlDatabase db = *(pWebSocketServer->database());
	
	{
		QSqlQuery query(db);
		query.prepare("SELECT count(*) as cnt FROM users_quests_answers uqa "
			" INNER JOIN users u ON u.id = uqa.userid"
			" INNER JOIN quest q ON q.idquest = uqa.questid"
			" " + where
		);
		foreach(QString key, filter_values.keys() ){
			query.bindValue(key, filter_values.value(key));
		}
		query.exec();
		if (query.next()) {
			QSqlRecord record = query.record();
			jsonData["count"] = record.value("cnt").toInt();
		}
	}

	// data
	QJsonArray answerlist;
	{
		QSqlQuery query(db);
		query.prepare("SELECT "
			" 	uqa.dt,"
			"	uqa.passed,"
			"	uqa.questid,"
			"	uqa.userid,"
			"	uqa.user_answer,"
			"	uqa.quest_answer,"
			"	uqa.levenshtein,"
			" 	u.nick,"
			"	u.logo,"
			"	u.email, "
			" 	q.name,"
			"	q.subject,"
			"	q.score,"
			"	q.count_user_solved"
			" FROM users_quests_answers uqa "
			" INNER JOIN users u ON u.id = uqa.userid"
			" INNER JOIN quest q ON q.idquest = uqa.questid"
			" " + where + 
			" ORDER BY dt DESC "
			" LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage)
		);
		foreach(QString key, filter_values.keys() ){
			query.bindValue(key, filter_values.value(key));
		}
		query.exec();
		while (query.next()) {
			QSqlRecord record = query.record();
			QJsonObject answer;
			answer["dt"] = record.value("dt").toString();
			answer["user_answer"] = record.value("user_answer").toString().toHtmlEscaped(); // TODO htmlspecialchars
			answer["quest_answer"] = record.value("quest_answer").toString().toHtmlEscaped(); // TODO htmlspecialchars
			answer["levenshtein"] = record.value("levenshtein").toInt();
			answer["passed"] = record.value("passed").toString();
			
			QJsonObject quest; // TODO deprecated
			quest["id"] = record.value("questid").toInt();
			quest["name"] = record.value("name").toString().toHtmlEscaped();
			quest["score"] = record.value("score").toInt();
			quest["subject"] = record.value("subject").toString();
			quest["count_user_solved"] = record.value("count_user_solved").toInt();
			answer["quest"] = quest;

			QJsonObject user; // TODO deprecated
			user["id"] = record.value("userid").toInt();
			user["logo"] = record.value("logo").toString();
			user["nick"] = record.value("nick").toString().toHtmlEscaped();
			user["email"] = record.value("email").toString().toHtmlEscaped();
			answer["user"] = user;

			answerlist.push_back(answer);
		}
	}

	jsonData["result"] = QJsonValue("DONE");
	jsonData["data"] = answerlist;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
