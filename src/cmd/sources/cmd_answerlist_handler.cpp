#include <cmd_answerlist_handler.h>
#include <QJsonArray>

CmdAnswerListHandler::CmdAnswerListHandler(){
	m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
	m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows on page"));
	m_vInputs.push_back(CmdInputDef("questid").optional().integer_().description("Filter for questid"));
	m_vInputs.push_back(CmdInputDef("userid").optional().integer_().description("Filter for userid"));
}

std::string CmdAnswerListHandler::cmd(){
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

const std::vector<CmdInputDef> &CmdAnswerListHandler::inputs(){
	return m_vInputs;
};

QString CmdAnswerListHandler::description(){
	return "Return user answer list";
}

QStringList CmdAnswerListHandler::errors(){
	QStringList	list;
	return list;
}

void CmdAnswerListHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	jsonData["m"] = QJsonValue(m);

	int nPage = obj["page"].toInt();
	jsonData["page"] = nPage;
	
	int nOnPage = obj["onpage"].toInt();
	jsonData["onpage"] = nOnPage;

	if(nOnPage > 50){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::OnPageCouldNotBeMoreThen50());
		return;
	}

	QStringList filters;
	QMap<QString,QString> filter_values;

	if(obj.contains("userid")){
		int userid = obj["userid"].toInt();
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
		int questid = obj["questid"].toInt();
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
