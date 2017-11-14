#include <cmd_game_update_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameUpdateHandler::CmdGameUpdateHandler(){
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Name of the Game"));
    m_vInputs.push_back(CmdInputDef("description").string_().required().description("Description of the Game"));
    m_vInputs.push_back(CmdInputDef("state").string_().required().description("State of the game"));
    m_vInputs.push_back(CmdInputDef("form").string_().required().description("Form of the game"));
    m_vInputs.push_back(CmdInputDef("type").string_().required().description("Type of the game"));
    m_vInputs.push_back(CmdInputDef("date_start").string_().required().description("Date start"));
    m_vInputs.push_back(CmdInputDef("date_stop").string_().required().description("Date stop"));
    m_vInputs.push_back(CmdInputDef("date_restart").string_().required().description("Date restart"));
    m_vInputs.push_back(CmdInputDef("organizators").string_().required().description("Organizators"));
}

QString CmdGameUpdateHandler::cmd(){
    return "game_update";
}

bool CmdGameUpdateHandler::accessUnauthorized(){
	return false;
}

bool CmdGameUpdateHandler::accessUser(){
	return false;
}

bool CmdGameUpdateHandler::accessTester(){
	return false;
}

bool CmdGameUpdateHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdGameUpdateHandler::inputs(){
	return m_vInputs;
};

QString CmdGameUpdateHandler::description(){
	return "Create the quest";
}

QStringList CmdGameUpdateHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameUpdateHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
    QJsonObject jsonData;

    QString sUuid = obj["uuid"].toString().trimmed();
    QString sName = obj["name"].toString().trimmed();
    QString sDescription = obj["description"].toString().trimmed();
    QString sState = obj["state"].toString().trimmed();
    QString sForm = obj["form"].toString().trimmed();
    QString sType = obj["type"].toString().trimmed();
    QString sDateStart = obj["date_start"].toString().trimmed();
    QString sDateStop = obj["date_stop"].toString().trimmed();
    QString sDateRestart = obj["date_restart"].toString().trimmed();
    QString sOrganizators = obj["organizators"].toString().trimmed();


    // original values
    QString sNamePrev = "";
    QString sTypePrev = "";
    QString sDescriptionPrev = "";
    QString sStatePrev = "";
    QString sFormPrev = "";
    QString sOrganizatorsPrev = "";
    QString sDateStartPrev = "";
    QString sDateStopPrev = "";
    QString sDateRestartPrev = "";

    QSqlDatabase db = *(pWebSocketServer->database());
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE uuid = :gameuuid");
        query.bindValue(":gameuuid", sUuid);

        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            sNamePrev = record.value("title").toString();
            sTypePrev = record.value("type_game").toString();
            sDescriptionPrev = record.value("description").toString();
            sStatePrev = record.value("state").toString();
            sFormPrev = record.value("form").toString();
            sOrganizatorsPrev = record.value("organizators").toString();
            sDateStartPrev = record.value("date_start").toString();
            sDateStopPrev = record.value("date_stop").toString();
            sDateRestartPrev = record.value("date_restart").toString();
        } else {
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Game not found"));
            return;
        }
    }

    if(sNamePrev != sName){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET title = :name WHERE uuid = :gameuuid");
        query.bindValue(":name", sName);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated name of game {" + sUuid + "} from [" + sNamePrev + "] to [" + sName + "]");
        sNamePrev = sName;
    }

    if(sTypePrev != sType){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET type_game = :type_game WHERE uuid = :gameuuid");
        query.bindValue(":type_game", sType);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated type of game {" + sUuid + "} from [" + sTypePrev + "] to [" + sType + "]");
        sTypePrev = sType;
    }

    if(sDescriptionPrev != sDescription){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET description = :description WHERE uuid = :gameuuid");
        query.bindValue(":description", sDescription);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated description of the game {" + sUuid + "}");
        sDescriptionPrev = sDescription;
    }

    if(sStatePrev != sState){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `state` = :state WHERE uuid = :gameuuid");
        query.bindValue(":state", sState);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated state of game {" + sUuid + "} from [" + sStatePrev + "] to [" + sState + "]");
        sStatePrev = sState;
    }

    if(sFormPrev != sForm){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `form` = :form WHERE uuid = :gameuuid");
        query.bindValue(":form", sForm);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated form of game {" + sUuid + "} from [" + sFormPrev + "] to [" + sForm + "]");
        sFormPrev = sForm;
    }

    if(sOrganizatorsPrev != sOrganizators){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `organizators` = :organizators WHERE uuid = :gameuuid");
        query.bindValue(":organizators", sOrganizators);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated organizators of game {" + sUuid + "}");
        sOrganizatorsPrev = sOrganizators;
    }

    if(sDateStartPrev != sDateStart){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `date_start` = :date_start WHERE uuid = :gameuuid");
        query.bindValue(":date_start", sDateStart);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated date start of game {" + sUuid + "} from [" + sDateStartPrev + "] to [" + sDateStart + "]");
        sDateStartPrev = sDateStart;
    }

    if(sDateStopPrev != sDateStop){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `date_stop` = :date_stop WHERE uuid = :gameuuid");
        query.bindValue(":date_stop", sDateStop);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated date stop of game {" + sUuid + "} from [" + sDateStopPrev + "] to [" + sDateStop + "]");
        sDateStopPrev = sDateStop;
    }

    if(sDateRestartPrev != sDateRestart){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `date_restart` = :date_restart WHERE uuid = :gameuuid");
        query.bindValue(":date_restart", sDateRestart);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated date stop of game {" + sUuid + "} from [" + sDateRestartPrev + "] to [" + sDateRestart + "]");
        sDateRestartPrev = sDateRestart;
    }

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(cmd());
    jsonResponse["result"] = QJsonValue("DONE");
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["data"] = jsonData;
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}
