#include "headers/cmd_answerlist_handler.h"
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
	
	QString table = "active";
	if(obj.contains("table")){
		table = obj["table"].toString().trimmed();
	}
	
	if(table != "active" && table != "backup"){
		pWebSocketServer->sendMessageError(pClient, cmd(), Errors::ParamTableMustBeActiveOrBackup());
		return;
	}
	
	table = table == "active" ? "tryanswer" : "tryanswer_backup";

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
	
	int nOnPage = 15;
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
	
	if(obj.contains("gameid")){
		QJsonValueRef vGameID = obj["gameid"];
		if(!vGameID.isDouble()){
			pWebSocketServer->sendMessageError(pClient, cmd(), Errors::GameIDMustBeInteger());
			return;
		}
		int gameid = vGameID.toInt();
		filters << "(g.id = :gameid)";
		filter_values[":gameid"] = gameid;
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

	if(obj.contains("gamename")){
		QString gamename = obj["gamename"].toString().trimmed();
		if(gamename != ""){
			filters << "(g.title LIKE :gamename)";
			filter_values[":gamename"] = "%" + gamename + "%";
		}
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
			filters << "(ta.passed = :passed)";
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
		query.prepare("SELECT count(*) as cnt FROM " + table + " ta "
			" INNER JOIN users u ON u.id = ta.iduser"
			" INNER JOIN quest q ON q.idquest = ta.idquest"
			" INNER JOIN games g ON g.id = ta.gameid"
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
			" ta.datetime_try, ta.passed, ta.idquest, ta.iduser, ta.answer_try, ta.answer_real, ta.levenshtein,"
			" u.nick, u.logo, u.email, "
			" q.name, q.subject, q.score, q.count_user_solved, q.gameid, "
			" g.title "
			" FROM " + table + " ta "
			
			" INNER JOIN users u ON u.id = ta.iduser"
			" INNER JOIN quest q ON q.idquest = ta.idquest"
			" INNER JOIN games g ON g.id = ta.gameid"
			" " + where + 
			" ORDER BY datetime_try DESC "
			" LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage)
		);
		foreach(QString key, filter_values.keys() ){
			query.bindValue(key, filter_values.value(key));
		}
		query.exec();
		while (query.next()) {
			QSqlRecord record = query.record();
			QJsonObject answer;
			answer["dt"] = record.value("datetime_try").toString();
			answer["answer_try"] = record.value("answer_try").toString(); // TODO htmlspecialchars
			answer["answer_real"] = record.value("answer_real").toString(); // TODO htmlspecialchars
			answer["levenshtein"] = record.value("levenshtein").toInt();
			answer["passed"] = record.value("passed").toString();
			
			QJsonObject game; // TODO deprecated
			game["id"] = record.value("gameid").toInt();
			game["title"] = record.value("title").toString();
			answer["game"] = game;
			
			QJsonObject quest; // TODO deprecated
			quest["id"] = record.value("idquest").toInt();
			quest["name"] = record.value("name").toString();
			quest["score"] = record.value("score").toInt();
			quest["subject"] = record.value("subject").toString();
			quest["count_user_solved"] = record.value("count_user_solved").toInt();
			answer["quest"] = quest;

			QJsonObject user; // TODO deprecated
			user["id"] = record.value("iduser").toInt();
			user["logo"] = record.value("logo").toString();
			user["nick"] = record.value("nick").toString();
			user["email"] = record.value("email").toString();
			answer["game"] = user;

			answerlist.push_back(answer);
		}
	}

	jsonData["result"] = QJsonValue("DONE");
	jsonData["data"] = answerlist;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
