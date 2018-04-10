#include <cmd_user_delete_handler.h>
#include <log.h>
#include <employ_database.h>

CmdUserDeleteHandler::CmdUserDeleteHandler()
    : CmdHandlerBase("user_delete", "Method for deleting a user"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("User's id"));
    m_vInputs.push_back(CmdInputDef("password").required().string_().description("Admin's password"));
}

// ---------------------------------------------------------------------

void CmdUserDeleteHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString sAdminPassword = jsonRequest["password"].toString();

    IUserToken *pUserToken = pRequest->userToken();
    int nAdminUserID = pUserToken->userid();

    QSqlDatabase db = *(pDatabase->database());

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

    int sUserID = jsonRequest["userid"].toInt();

    // check if the user exists
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :id");
        query.bindValue(":id", sUserID);

        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        if(!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "User not found"));
            return;
        }
    }

    QSqlQuery query_del(db);

    // delete from feedback
    {
        query_del.prepare("DELETE FROM feedback WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from feedback_msg
    {
        query_del.prepare("DELETE FROM feedback_msg WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from quest
    {
        query_del.prepare("DELETE FROM quest WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_games
    {
        query_del.prepare("DELETE FROM users_games WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_profile
    {
        query_del.prepare("DELETE FROM users_profile WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_quests
    {
        query_del.prepare("DELETE FROM users_quests WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_tokens
    {
        query_del.prepare("DELETE FROM users_tokens WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_tokens_invalid
    {
        query_del.prepare("DELETE FROM users_tokens_invalid WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_offers
    {
        query_del.prepare("DELETE FROM users_offers WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from quests_proposal
    {
        query_del.prepare("DELETE FROM quests_proposal WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_quests_answers
    {
        query_del.prepare("DELETE FROM users_quests_answers WHERE userid = :userid");
        query_del.bindValue(":userid", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users
    {
        query_del.prepare("DELETE FROM users WHERE id = :id");
        query_del.bindValue(":id", sUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

