
/*********************************************
 * User info (deprecated)
**********************************************/

Jsonrpc2UserRetrive::Jsonrpc2UserRetrive()
    : WsjcppJsonrpc2Base("user", "Return user info") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);
    setDeprecatedFromVersion("0.2.32");

    // validation and description input fields
    optionalIntegerParam("userid", "Id of user");
}

// ---------------------------------------------------------------------

void Jsonrpc2UserRetrive::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse;

    WsjcppUserSession *pUserSession = pRequest->getUserSession();

    if (jsonRequest.find("userid") != jsonRequest.end() && pUserSession == nullptr) {
        pRequest->sendMessageError(cmd(), WsjcppError(401, "Not Authorized Request"));
        return;
    }

    bool bCurrentUserOrAdmin = false;

    int nUserID = 0;
    if (pUserSession != nullptr) {
        nUserID = pUserSession->userid();
        bCurrentUserOrAdmin = true;
    }

    if (jsonRequest.find("userid") != jsonRequest.end()) {
        int nUserID_ = jsonRequest.at("userid").get<int>();
        if (nUserID_ != nUserID) {
            bCurrentUserOrAdmin = pRequest->isAdmin();
        }
        nUserID = nUserID_;
    }

    nlohmann::json data;
    nlohmann::json profile;
    QSqlDatabase db = *(pDatabase->database());

    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :userid");
        query.bindValue(":userid", nUserID);
        query.exec();

        if (query.next()) {
            QSqlRecord record = query.record();
            data["id"] = record.value("id").toInt();
            data["uuid"] = record.value("uuid").toString().toStdString();
            data["nick"] = record.value("nick").toString().toHtmlEscaped().toStdString();
            data["role"] = record.value("role").toString().toStdString();
            data["logo"] = record.value("logo").toString().toHtmlEscaped().toStdString();
            data["about"] = record.value("about").toString().toHtmlEscaped().toStdString();
            data["status"] = record.value("status").toString().toStdString();
            data["rating"] = record.value("rating").toString().toStdString();
            data["university"] = record.value("university").toString().toHtmlEscaped().toStdString();

            if (bCurrentUserOrAdmin) {
                data["email"] = record.value("email").toString().toStdString();
                data["dt_create"] = record.value("dt_create").toString().toStdString();
                data["dt_last_login"] = record.value("dt_last_login").toString().toStdString();
                data["last_ip"] = record.value("last_ip").toString().toStdString();
                data["country"] = record.value("country").toString().toStdString();
                data["region"] = record.value("region").toString().toStdString();
                data["city"] = record.value("city").toString().toStdString();
            }
        } else {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found user"));
            return;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("SELECT name, value, date_change FROM users_profile WHERE userid = :userid");
        query.bindValue(":userid", nUserID);
        query.exec();

        while (query.next()) {
            QSqlRecord record = query.record();
            std::string name = record.value("name").toString().toStdString();
            std::string value = record.value("value").toString().toStdString();
            profile[name] = value;

            // TODO clenup 'template' from user profiles
        }
    }

    jsonResponse["data"] = data;
    jsonResponse["profile"] = profile;
    jsonResponse["access"] = bCurrentUserOrAdmin;
    pRequest->done(jsonResponse);
}