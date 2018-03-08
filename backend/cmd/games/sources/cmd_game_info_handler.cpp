#include <cmd_game_info_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameInfoHandler::CmdGameInfoHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

// ---------------------------------------------------------------------

std::string CmdGameInfoHandler::cmd(){
    return "game_info";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGameInfoHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGameInfoHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGameInfoHandler::description(){
    return "Return game info";
}

// ---------------------------------------------------------------------

void CmdGameInfoHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QJsonObject data;

    QString sUuid = jsonRequest["uuid"].toString().trimmed();

    QSqlDatabase db = *(pRequest->server()->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM games WHERE uuid = :gameuuid");
    query.bindValue(":gameuuid", sUuid);

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    if (query.next()) {
        QSqlRecord record = query.record();
        data["uuid"] = record.value("uuid").toString();
        data["title"] = record.value("title").toString();
        data["type_game"] = record.value("type_game").toString();
        data["date_start"] = record.value("date_start").toString();
        data["date_stop"] = record.value("date_stop").toString();
        data["date_restart"] = record.value("date_restart").toString();
        data["description"] = record.value("description").toString();
        data["state"] = record.value("state").toString();
        data["form"] = record.value("form").toString();
        data["organizators"] = record.value("organizators").toString();
        data["maxscore"] = record.value("maxscore").toInt();
    } else {
        pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
        return;
    }

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
