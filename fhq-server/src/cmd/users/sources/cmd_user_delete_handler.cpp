#include <cmd_user_delete_handler.h>
#include <log.h>

CmdUserDeleteHandler::CmdUserDeleteHandler(){
    TAG = "CmdUsersDeleteHandler";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").required().uuid_().description("User's uuid"));
    m_vInputs.push_back(CmdInputDef("password").required().string_().description("Admin's password"));
}

// ---------------------------------------------------------------------

std::string CmdUserDeleteHandler::cmd(){
    return "user_delete";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdUserDeleteHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdUserDeleteHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdUserDeleteHandler::description(){
    return "Method for deleting a user";
}

// ---------------------------------------------------------------------

void CmdUserDeleteHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString sAdminPassword = jsonRequest["password"].toString();

    IUserToken *pUserToken = pRequest->userToken();
    int nAdminUserID = pUserToken->userid();

    QSqlDatabase db = *(pRequest->server()->database());

    // check admin password
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :userid");
        query.bindValue(":userid", nAdminUserID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        QString sPass = "";
        QString sEmail = "";

        if (query.next()) {
            QSqlRecord record = query.record();
            sEmail = record.value("email").toString();
            sPass = record.value("pass").toString();
        }else{
            pRequest->sendMessageError(cmd(), Errors::NotFound("user"));
            return;
        }

        QString sAdminPasswordHash = sEmail.toUpper() + sAdminPassword;
        sAdminPasswordHash = QString("%1").arg(QString(QCryptographicHash::hash(sAdminPasswordHash.toUtf8(),QCryptographicHash::Sha1).toHex()));

        if(sAdminPasswordHash != sPass){
            pRequest->sendMessageError(cmd(), Error(401, "Wrong password"));
            return;
        }
    }

    QString sUuid = jsonRequest["uuid"].toString().trimmed();

    int nUserID = 0;

    // check if the user exists
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE uuid = :uuid");
        query.bindValue(":uuid", sUuid);

        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        if(!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "User not found"));
            return;
        }else{
            QSqlRecord record = query.record();
            nUserID = record.value("id").toInt();
        }
    }

    QSqlQuery query_del(db);

    // delete from feedback
    {
        query_del.prepare("DELETE FROM feedback WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from feedback_msg
    {
        query_del.prepare("DELETE FROM feedback_msg WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from quest
    {
        query_del.prepare("DELETE FROM quest WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_games
    {
        query_del.prepare("DELETE FROM users_games WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_profile
    {
        query_del.prepare("DELETE FROM users_profile WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_quests
    {
        query_del.prepare("DELETE FROM users_quests WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_tokens
    {
        query_del.prepare("DELETE FROM users_tokens WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_tokens_invalid
    {
        query_del.prepare("DELETE FROM users_tokens_invalid WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_offers
    {
        query_del.prepare("DELETE FROM users_offers WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from quests_proposal
    {
        query_del.prepare("DELETE FROM quests_proposal WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_quests_answers
    {
        query_del.prepare("DELETE FROM users_quests_answers WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users
    {
        query_del.prepare("DELETE FROM users WHERE uuid = :uuid");
        query_del.bindValue(":uuid", sUuid);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

