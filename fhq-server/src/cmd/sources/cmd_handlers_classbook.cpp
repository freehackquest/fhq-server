#include <cmd_handlers_classbook.h>
#include <log.h>
#include <runtasks.h>
#include <log.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>
#include <SmtpMime>
#include <quazip.h>
#include <quazipfile.h>
#include <quazipfileinfo.h>
#include <utils_merge_text.h>

// *******************************************
// * This handler will be add classbook record
// *******************************************

CmdClassbookAddRecordHandler::CmdClassbookAddRecordHandler()
    : CmdHandlerBase("classbook_add_record", "Adds a new article with the specified name, content, and id."){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("parentid").required().integer_().description("pareintid for classbook article"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("name of article"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("content of article"));
    m_vInputs.push_back(CmdInputDef("uuid").optional().uuid_().description("uuid of article"));
    m_vInputs.push_back(CmdInputDef("ordered").optional().integer_().description("order of article"));
}

// ---------------------------------------------------------------------

void CmdClassbookAddRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int parentid = jsonRequest["parentid"].toInt();

    //Check parentid in database
    QSqlQuery query(db);
    if(parentid !=0){
        query.prepare("SELECT name FROM classbook WHERE id=:parentid");
        query.bindValue(":parentid", parentid);
        query.exec();
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Errors::NotFound("article with this id"));
            return;
        }
    }

    QString name = jsonRequest["name"].toString().trimmed().toHtmlEscaped();
    QString content = jsonRequest["content"].toString().trimmed().toHtmlEscaped();

    //Set uuid from request if available, else generate uuid
    QString uuid;
    if(jsonRequest.contains("uuid")){
        query.prepare("SELECT uuid FROM classbook WHERE uuid=:uuid");
        query.bindValue(":uuid", jsonRequest["uuid"].toString());
        query.exec();
        if (!query.next()){
            uuid = jsonRequest["uuid"].toString().replace("{", "").replace("}", "");
        } else {
            pRequest->sendMessageError(cmd(), Error(403, "Uuid already exist"));
            return;
        }
    } else {
        uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    }

    //Set md5_content hash
    QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());

    //Find parentuuid from database
    QString parentuuid = "00000000-0000-0000-0000-000000000000";
    if(parentid != 0){
        query.prepare("SELECT uuid FROM classbook WHERE id=:parentid");
        query.bindValue(":parentid", parentid);
        query.exec();
        query.next();
        QSqlRecord record = query.record();
        parentuuid = record.value("uuid").toString();
    }

    //Set ordered of article: increment max of child's ordered
    int ordered;
    if (jsonRequest.contains("ordered")){
        ordered = jsonRequest["ordered"].toInt();
    } else {
        query.prepare("SELECT MAX(ordered) AS max FROM classbook WHERE parentid=:parentid");
        query.bindValue(":parentid", parentid);
        query.exec();
        QSqlRecord record = query.record();
        if (!record.value("max").isNull())
            ordered = record.value("max").toInt() + 1;
        else {
            if (parentid != 0){
                query.prepare("SELECT ordered FROM classbook WHERE id=:parentid");
                query.bindValue(":parentid", parentid);
                query.exec();
                if (query.next()){
                    QSqlRecord record = query.record();
                    ordered = record.value("ordered").toInt() + 1;
                } else {
                    pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
                    return;
                }
            } else {
                ordered = 1;
            }

        }
    }

    //Insert article into classbook
    query.prepare("INSERT INTO classbook("
                  "parentid,"
                  "ordered,"
                  "uuid,"
                  "parentuuid,"
                  "name,"
                  "content,"
                  "md5_content,"
                  "created,"
                  "updated"
                  ")"
                  "VALUES("
                  ":parentid,"
                  ":ordered,"
                  ":uuid,"
                  ":parentuuid,"
                  ":name,"
                  ":content,"
                  ":md5_content,"
                  "NOW(),"
                  "NOW()"
                  ")");
    query.bindValue(":parentid", parentid);
    query.bindValue(":ordered", ordered);
    query.bindValue(":uuid", uuid);
    query.bindValue(":parentuuid", parentuuid);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    query.bindValue(":md5_content", md5_content);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    QJsonObject data;
    int rowid = query.lastInsertId().toInt();
    data["classbookid"] = QJsonValue(rowid);
    data["parentid"] = parentid;
    data["name"] = name;
    data["content"] = content;
    data["md5_content"] = md5_content;

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// * This handler will be delete classbook record
// *******************************************

CmdClassbookDeleteRecordHandler::CmdClassbookDeleteRecordHandler()
    : CmdHandlerBase("classbook_delete_record", "Delete a article with a given classbookid"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("id for classbook article"));
}

// ---------------------------------------------------------------------

void CmdClassbookDeleteRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int classbookid = jsonRequest["classbookid"].toInt();

    //DELETE Record IF haven't childs
    QSqlQuery query(db);
    if(classbookid !=0){
        query.prepare("SELECT id FROM classbook WHERE parentid=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if (query.next()){
            pRequest->sendMessageError(cmd(), Error(403, "Could not delete, because childs exists. Please remove childs first."));
            return;
        }
        //Delete record in classbook
        query.prepare("DELETE FROM classbook WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Errors::DatabaseError(query.lastError().text()));
            return;
        }

        //Delete record's localization
        query.prepare("DELETE FROM classbook_localization WHERE classbookid=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Errors::DatabaseError(query.lastError().text()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// * This handler will be export classbook record
// *******************************************

CmdClassbookExportHandler::CmdClassbookExportHandler()
    : CmdHandlerBase("classbook_export", "Export classbook's articles to html or markdown, optionally in zip archive."){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("output").required().string_().description("The output file format"));
    m_vInputs.push_back(CmdInputDef("lang").required().string_().description("The output file format"));
    m_vInputs.push_back(CmdInputDef("zip").optional().bool_().description("Zipping the output"));
}

// ---------------------------------------------------------------------

void CmdClassbookExportHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    QString lang = jsonRequest.value("lang").toString().trimmed();
    QString output = jsonRequest.value("output").toString().trimmed();
    //Check parametrs
    if (output != "html" && output != "markdown"){
        pRequest->sendMessageError(cmd(), Error(403, "The output is not supported."));
        return;
    }
    QList<QString> langs;
    langs << "en" << "de" << "ru";
    if (!langs.contains(jsonRequest.value("lang").toString())){
        pRequest->sendMessageError(cmd(), Error(403, "The language is not supported."));
        return;
    }

    QString tmpFileDir = QDir::tempPath();
    QString tmpFile = tmpFileDir + "/freehackquest-classbook_export_file";
    QFile file(tmpFile);
    file.open(QIODevice::WriteOnly);
    if (output == "html"){
        createHtml(&file, lang, query);
    }
    if (output == "markdown"){
        createMD(&file, lang, query);
    }
    file.close();
    file.open(QIODevice::ReadOnly);

    if (jsonRequest.contains("zip") && jsonRequest.value("zip").toBool()){
        QString tmpDir = QDir::tempPath();
        QString tmpZipFile = tmpDir + "/freehackquest-classbook_" + lang + ".zip";
        // prepare zip archive
        QuaZip zip(tmpZipFile);
        zip.open(QuaZip::mdCreate);
        QuaZipFile export_zipfile(&zip);

        QString name = "freehackquest-classbook." + output;
        export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(name));
        // After .toString(), you should specify a text codec to use to encode the
        // string data into the (binary) file. Here, I use UTF-8:
        export_zipfile.write(file.readAll());
        export_zipfile.close();
        zip.close();
        QFile fileZip(tmpZipFile);
        if (!fileZip.open(QIODevice::ReadOnly)){
            pRequest->sendMessageError(cmd(), Error(500, "Could not open zip file"));
            return;
        }
        QByteArray baZip = fileZip.readAll();
        QJsonObject jsonData;
        jsonData["zipfile_base64"] = QString(baZip.toBase64());
        jsonData["zipfile_name"] = name;
        jsonResponse["data"] = jsonData;
        fileZip.close();
        fileZip.remove();
    } else {
        jsonResponse["data"] = QString::fromUtf8(file.readAll());
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
    file.close();
    file.remove();
}

void CmdClassbookExportHandler::createHtml(QFile *file, QString lang, QSqlQuery query){
    QTextStream out(file);
    out.setCodec("UTF-8");
    QMap <int, QString> name_of_articles;
    if (lang == "en"){
        query.prepare("SELECT id, name FROM classbook ORDER BY ordered");
        query.exec();
        while (query.next()){
            QSqlRecord record = query.record();
            int id = record.value("id").toInt();
            name_of_articles[id] = record.value("name").toString();
        }
    } else {
        query.prepare("SELECT classbookid, name FROM classbook_localization WHERE lang=:lang");
        query.bindValue(":lang", lang);
        query.exec();
        while (query.next()){
            QSqlRecord record = query.record();
            int id = record.value("classbookid").toInt();
            name_of_articles[id] = record.value("name").toString();
        }
    }

    out << "<!DOCTYPE HTML><html><head><title>Freehackquest classbook</title>" << endl;
    out << "<meta charset=\"utf-8\"></head>" << endl;
    out << "<body><h1> Freehackquest Classbook</h1>" << endl;
    out << "<h2>Table of contents</h2>" << endl;
    QMap <int, QString>::iterator i;
    for (i = name_of_articles.begin(); i != name_of_articles.end(); ++i){
        out << "<h3><a href=#" << i.key() << ">" << i.value().toUtf8() << "</a></h3>" << endl;
    }

    if (lang == "en"){
        query.prepare("SELECT id, name, content FROM classbook ORDER BY ordered");
        query.exec();
        while (query.next()){
            QSqlRecord record = query.record();
            out << "<article id=" << record.value("id").toInt() << "><h2>";
            out << record.value("name").toString().toUtf8() << "</h2>" << endl;
            out << "<p>" << record.value("content").toString().toUtf8() << "</p></article>" << endl;
        }
    } else {
        query.prepare("SELECT classbookid, name, content FROM classbook_localization WHERE lang=:lang");
        query.bindValue(":lang", lang);
        query.exec();
        while (query.next()){
            QSqlRecord record = query.record();
            out << "<article><h2 id =" << record.value("classbookid").toInt() << ">";
            out << record.value("name").toString().toUtf8() << "</h2>" << endl;
            out << "<p>" << record.value("content").toString().toUtf8() << "</p></article>" << endl;
        }
    }
    out << "</body></html>" << endl;
}

void CmdClassbookExportHandler::createMD(QFile *file, QString lang, QSqlQuery query){
    QTextStream out(file);
    out.setCodec("UTF-8");
    QList<QString> name_of_articles;
    if (lang == "en"){
        query.prepare("SELECT name FROM classbook ORDER BY ordered");
        query.exec();
        while (query.next()){
            QSqlRecord record = query.record();
            name_of_articles << record.value("name").toString();
        }
    } else {
        query.prepare("SELECT name FROM classbook_localization WHERE lang=:lang");
        query.bindValue(":lang", lang);
        query.exec();
        while (query.next()){
            QSqlRecord record = query.record();
            name_of_articles << record.value("name").toString();
        }
    }

    out << "# Freehackquest classbook" << endl;
    out << "## Table of content" << endl;
    for (int i = 0; i < name_of_articles.size(); ++i){
        out << "* " << name_of_articles.at(i).toUtf8() << endl;
    }

    if (lang == "en"){
        query.prepare("SELECT name, content FROM classbook ORDER BY ordered");
        query.exec();
        while (query.next()){
            QSqlRecord record = query.record();
            out << "### " << record.value("name").toString().toUtf8() << endl;
            out << record.value("content").toString().toUtf8() << endl;
        }
    } else {
        query.prepare("SELECT name, content FROM classbook_localization WHERE lang=:lang");
        query.bindValue(":lang", lang);
        query.exec();
        while (query.next()){
            QSqlRecord record = query.record();
            out << "### " << record.value("name").toString().toUtf8() << endl;
            out << record.value("content").toString().toUtf8() << endl;
        }
    }
}

// *******************************************
// * This handler will be return classbook record info
// *******************************************

CmdClassbookInfoHandler::CmdClassbookInfoHandler()
    : CmdHandlerBase("classbook_info", "Return name and content, langs, path classbook article with a given id"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("id for the classbook article"));
    m_vInputs.push_back(CmdInputDef("lang").optional().string_().description("Set lang for the article"));
}

// ---------------------------------------------------------------------

void CmdClassbookInfoHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int classbookid = jsonRequest["classbookid"].toInt();

    QSqlQuery query(db);
    QJsonObject info;

    //GET parentid and uuid for the article
    query.prepare("SELECT parentid, uuid FROM classbook WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    query.exec();
    if (query.next()) {
        QSqlRecord record = query.record();
        info["classbookid"] = classbookid;
        info["parentid"] = record.value("parentid").toInt();
        info["uuid"] = record.value("uuid").toString();
    } else {
        pRequest->sendMessageError(cmd(), Errors::NotFound("the article"));
        return;
    }

    //SET lang
    QString lang;
    if (jsonRequest.contains("lang")){
        lang = jsonRequest.value("lang").toString().trimmed();
        QList<QString> allow_lang = {"en", "ru","de"};
        if(!allow_lang.contains(lang)){
            pRequest->sendMessageError(cmd(), Error(404, "Language is not support"));
            return;
        }
    } else {
        lang = "en";
    }

    //GET localization
    if(lang != "en"){
        //GET localization for the article with a given lang
        lang = jsonRequest.value("lang").toString().trimmed();
        query.prepare("SELECT name, content FROM classbook_localization WHERE classbookid=:classbookid AND lang=:lang");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":lang", lang);
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            info["lang"] = QJsonValue(lang);
            info["name"] = record.value("name").toString();
            info["content"] = record.value("content").toString();
        } else {
            //GET default localization for the article
            query.prepare("SELECT name, content FROM classbook WHERE id=:classbookid");
            query.bindValue(":classbookid", classbookid);
            query.exec();
            if (query.next()) {
                QSqlRecord record = query.record();
                info["lang"] = QJsonValue("en");
                info["name"] = record.value("name").toString();
                info["content"] = record.value("content").toString();
            } else {
                pRequest->sendMessageError(cmd(), Errors::NotFound("the article"));
                return;
            }
        }
    } else {
        //GET default localization for the article
        query.prepare("SELECT name, content FROM classbook WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            info["lang"] = QJsonValue(lang);
            info["name"] = record.value("name").toString();
            info["content"] = record.value("content").toString();
        } else {
            pRequest->sendMessageError(cmd(), Errors::NotFound("the article"));
            return;
        }
    }

    //FIND langs for the article
    QJsonObject langs;
    query.prepare("SELECT id, lang FROM classbook_localization WHERE classbookid=:classbookid");
    query.bindValue(":classbookid", classbookid);
    query.exec();
    if (query.next()) {
        QSqlRecord record = query.record();
        QString local_lang;
        local_lang = record.value("lang").toString();
        langs[local_lang] = record.value("id").toInt();
    }
    info["langs"] = langs;

    //FIND parents for the article
    QJsonArray parents;
    QSet<int> set_of_parent;
    int parentid = info.value("parentid").toInt();
    for (int i=0; i < 5; ++i){
        //END IT root article
        if (parentid==0){
            QJsonObject parent;
            parent["classbookid"] = 0;
            parent["parentid"] = 0;
            parent["name"] = "Root";
            parents.push_back(parent);
            break;
        }
        //CONTINUE if already have a article in parents
        if (set_of_parent.contains(parentid)){
            continue;
        }
        query.prepare("SELECT id, name, parentid FROM classbook WHERE id=:parentid");
        query.bindValue(":parentid", parentid);
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            QJsonObject parent;
            parent["classbookid"] = record.value("id").toInt();
            if (classbookid == parent["classbookid"].toInt()){
                continue;
            }
            parentid = record.value("parentid").toInt();
            parent["parentid"] = parentid;
            parent["name"] = record.value("name").toString();
            parents.push_back(parent);
            set_of_parent.insert(parent["classbookid"].toInt());
        } else {
            pRequest->sendMessageError(cmd(), Error(404, "Error in PATHFINDER. Not found the article with a given classbookid"));
            return;
        }
    }
    //ADD parents to response
    info["parents"] = parents;

    jsonResponse["data"] = info;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// * This handler will be return classbook records list
// *******************************************


CmdClassbookListHandler::CmdClassbookListHandler()
    : CmdHandlerBase("classbook_list", "Return list of classbook articles with parentid, id, names, childs, proposals for a given parentid"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("parentid").required().integer_().description("parentid for classbook articles"));
    m_vInputs.push_back(CmdInputDef("lang").optional().string_().description("lang for classbook articles"));
    m_vInputs.push_back(CmdInputDef("search").optional().string_().description("Search string for classbook articles"));
}

// ---------------------------------------------------------------------

void CmdClassbookListHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    int parentid = jsonRequest["parentid"].toInt();
    QJsonArray data;

    if (jsonRequest.contains("search") && !jsonRequest.value("search").toString().isEmpty()){
        QString search = jsonRequest.value("search").toString();

        //SET lang
        QString lang;
        if (jsonRequest.contains("lang")){
            lang = jsonRequest.value("lang").toString().trimmed();
            QList<QString> allow_lang = {"en", "ru","de"};
            if(!allow_lang.contains(lang)){
                pRequest->sendMessageError(cmd(), Error(404, "Language is'not support"));
                return;
            }
        } else {
            lang = "en";
        }

        QSqlQuery query1(db);
        if (lang == "en"){
            query1.prepare("SELECT id, name FROM classbook WHERE name LIKE :search OR content LIKE :search ORDER BY ordered;");
            query1.bindValue(":search", "%" + search + "%");
        } else {
            query1.prepare("SELECT id, name FROM classbook_localization WHERE name LIKE :search OR content LIKE :search");
            query1.bindValue(":search", "%" + search + "%");
        }

        if (!query1.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query1.lastError().text()));
            return;
        }
        while (query1.next()) {
            QSqlRecord record = query1.record();
            QJsonObject item;
            int classbookid;
            item["parentid"] = parentid;
            classbookid = record.value("id").toInt();
            item["classbookid"] = classbookid;
            if (lang != "en"){
                query.prepare("SELECT classbookid, name FROM classbook_localization "
                                  "WHERE lang=:lang AND (name LIKE '%:search%' OR content LIKE '%:search%')");
                query.bindValue(":search", search);
                query.bindValue(":lang", lang);
            }
            item["name"] = record.value("name").toString();

            //COUNT childs for an article
            QSqlQuery query_childs(db);
            query_childs.prepare("SELECT COUNT(id) AS childs FROM classbook WHERE parentid =:classbookid");
            query_childs.bindValue(":classbookid", classbookid);
            query_childs.exec();
            int childs = 0;
            if (query_childs.next()){
                QSqlRecord record_childs = query_childs.record();
                childs = record_childs.value("childs").toInt();
            }
            item["childs"] = QJsonValue(childs);

            //COUNT proposals for an article
            QSqlQuery query_proposals(db);
            query_proposals.prepare("SELECT COUNT(id) AS proposals FROM classbook_proposal "
                                    "WHERE classbookid =:classbookid AND lang=:lang");
            query_proposals.bindValue(":classbookid", classbookid);
            query_proposals.bindValue(":lang", lang);
            query_proposals.exec();
            int proposals = 0;
            if (query_proposals.next()){
                QSqlRecord record_proposals = query_proposals.record();
                proposals = record_proposals.value("proposals").toInt();
            }
            item["proposals"] = QJsonValue(proposals);

            data.push_back(item);
        }

    } else {
    //CHECK exist parentid in DB
    query.prepare("SELECT name FROM classbook WHERE id =:parentid");
    query.bindValue(":parentid", parentid);
    query.exec();
    if (!query.next() && parentid != 0){
        pRequest->sendMessageError(cmd(), Error(404, "Not found the article with a given parentid"));
        return;
    }


    //SET lang
    QString lang;
    if (jsonRequest.contains("lang")){
        lang = jsonRequest.value("lang").toString().trimmed();
        QList<QString> allow_lang = {"en", "ru","de"};
        if(!allow_lang.contains(lang)){
            pRequest->sendMessageError(cmd(), Error(404, "Language is'not support"));
            return;
        }
    } else {
        lang = "en";
    }

    query.prepare("SELECT id, name FROM classbook WHERE parentid =:parentid ORDER BY ordered");
    query.bindValue(":parentid", parentid);
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        QJsonObject item;
        int classbookid;
        item["parentid"] = parentid;
        classbookid = record.value("id").toInt();
        item["classbookid"] = classbookid;

        //GET name with the lang
        if (lang == "en"){
            item["name"] = record.value("name").toString();
        } else {
            QSqlQuery query_lang(db);
            query_lang.prepare("SELECT name FROM classbook_localization WHERE classbookid=:classbookid AND lang=:lang");
            query_lang.bindValue(":classbookid", classbookid);
            query_lang.bindValue(":lang", lang);
            query_lang.exec();
            if (query_lang.next()){
                QSqlRecord record_lang = query_lang.record();
                item["name"] = record_lang.value("name").toString();
            } else {
                item["name"] = record.value("name").toString();
            }
        }

        //COUNT childs for an article
        QSqlQuery query_childs(db);
        query_childs.prepare("SELECT COUNT(id) AS childs FROM classbook WHERE parentid =:classbookid");
        query_childs.bindValue(":classbookid", classbookid);
        query_childs.exec();
        int childs = 0;
        if (query_childs.next()){
            QSqlRecord record_childs = query_childs.record();
            childs = record_childs.value("childs").toInt();
        }
        item["childs"] = QJsonValue(childs);

        //COUNT proposals for an article
        QSqlQuery query_proposals(db);
        query_proposals.prepare("SELECT COUNT(id) AS proposals FROM classbook_proposal "
                                "WHERE classbookid =:classbookid AND lang=:lang");
        query_proposals.bindValue(":classbookid", classbookid);
        query_proposals.bindValue(":lang", lang);
        query_proposals.exec();
        int proposals = 0;
        if (query_proposals.next()){
            QSqlRecord record_proposals = query_proposals.record();
            proposals = record_proposals.value("proposals").toInt();
        }
        item["proposals"] = QJsonValue(proposals);

        data.push_back(item);
    }}

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// * This handler will be update classbook record
// *******************************************

CmdClassbookUpdateRecordHandler::CmdClassbookUpdateRecordHandler()
    : CmdHandlerBase("classbook_update_record", "Update a article with a given classbookid"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("id for classbook article"));
    m_vInputs.push_back(CmdInputDef("name").optional().string_().description("name for classbook article"));
    m_vInputs.push_back(CmdInputDef("content").optional().string_().description("content for classbook article"));
    m_vInputs.push_back(CmdInputDef("ordered").optional().integer_().description("ordered for classbook article"));
    m_vInputs.push_back(CmdInputDef("parentid").optional().integer_().description("parentid for classbook article"));
}

// ---------------------------------------------------------------------

void CmdClassbookUpdateRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int classbookid = jsonRequest["classbookid"].toInt();
    //IF classbookid = 0, THEN reject request
    if(classbookid == 0){
        pRequest->sendMessageError(cmd(), Error(403, "Not today. It's root article id"));
        return;
    }

    //Find article with a given classbookid
    QSqlQuery query(db);
    query.prepare("SELECT name FROM classbook WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    query.exec();
    if (!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "Not found a article with a given classbookid"));
        return;
    }

    //CHECK Do we have anything to change?
    if (!(jsonRequest.contains("name") || jsonRequest.contains("content") || jsonRequest.contains("ordered") || jsonRequest.contains("parentid"))){
        pRequest->sendMessageError(cmd(), Error(403, "Not found a charges. Not enough parameters"));
        return;
    }

    int parentid;
    //FIND article with id = parentid AND UPDATE parentid IF exist
    if(jsonRequest.contains("parentid")){
        parentid = jsonRequest.value("parentid").toInt();

        if (parentid != 0){
            //CHECK existence of the article
            query.prepare("SELECT name FROM classbook WHERE id=:parentid");
            query.bindValue(":parentid", parentid);
            query.exec();
            if(!query.next()){
                pRequest->sendMessageError(cmd(), Error(404, "Not found a article with a given parentid"));
                return;
            }
        }

        query.prepare("UPDATE classbook SET parentid=:parentid WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":parentid", parentid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE name for article
    QString name;
    if(jsonRequest.contains("name")){
        name = jsonRequest.value("name").toString().trimmed().toHtmlEscaped();
        query.prepare("UPDATE classbook SET name=:name WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":name", name);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE content for article
    QString content;
    if(jsonRequest.contains("content")){
        content = jsonRequest.value("content").toString().trimmed().toHtmlEscaped();
        QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());
        query.prepare("UPDATE classbook SET content=:content, md5_content=:md5_content WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":content", content);
        query.bindValue(":md5_content", md5_content);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE ordered for article
    int ordered;
    if(jsonRequest.contains("ordered")){
        ordered = jsonRequest.value("ordered").toInt();
        query.prepare("UPDATE classbook SET ordered=:ordered WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":ordered", ordered);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE updated column
    query.prepare("UPDATE classbook SET updated = NOW() WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    //GET article info
    QJsonObject info;
    query.prepare("SELECT id, name, content, md5_content, ordered, parentid FROM classbook WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    query.exec();
    if (query.next()) {
        QSqlRecord record = query.record();
        info["classbookid"] = classbookid;
        info["parentid"] = record.value("parentid").toInt();
        info["name"] = record.value("name").toString();
        info["content"] = record.value("content").toString();
        info["md5_content"] = record.value("md5_content").toString();
        info["ordered"] = record.value("ordered").toInt();
    } else {
        pRequest->sendMessageError(cmd(), Errors::NotFound("article"));
        return;
    }

    jsonResponse["data"] = QJsonValue(info);
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// * This handler will be add classbook localization record
// *******************************************


CmdClassbookLocalizationAddRecordHandler::CmdClassbookLocalizationAddRecordHandler()
    : CmdHandlerBase("classbook_localization_add_record", "Add a new article localization for the English version"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("Classbookid for article localization"));
    m_vInputs.push_back(CmdInputDef("lang").required().string_().description("Language"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("Article name"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("The content of the article"));
}

// ---------------------------------------------------------------------

void CmdClassbookLocalizationAddRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int classbookid = jsonRequest["classbookid"].toInt();

    QSqlDatabase db = *(pDatabase->database());

    QJsonObject data;

    QSqlQuery query(db);
    QString lang = jsonRequest["lang"].toString().trimmed();
    query.prepare("SELECT lang FROM classbook_localization WHERE lang = :lang AND classbookid=:classbookid");
    query.bindValue(":lang", jsonRequest["lang"].toString().trimmed());
    query.bindValue(":classbookid", classbookid);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(query.next()){
        pRequest->sendMessageError(cmd(), Error(400, "This lang already exist"));
        return;
    }

    QString name = jsonRequest["name"].toString().trimmed();
    QString content = jsonRequest["content"].toString().trimmed();

    //Set md5_content hash
    QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());

    //generate uuid
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");

    query.prepare("INSERT INTO classbook_localization("
                  "classbookid,"
                  "uuid,"
                  "lang,"
                  "name,"
                  "content,"
                  "md5_content,"
                  "created,"
                  "updated"
                  ") "
                  "VALUES("
                  ":classbookid,"
                  ":uuid,"
                  ":lang,"
                  ":name,"
                  ":content,"
                  ":md5_content,"
                  "NOW(),"
                  "NOW()"
                  ")");
    query.bindValue(":classbookid", classbookid);
    query.bindValue(":uuid", uuid);
    query.bindValue(":lang", lang);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    query.bindValue(":md5_content", md5_content);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    int rowid = query.lastInsertId().toInt();
    data["classbookid"] = classbookid;
    data["classbook_localizationid"] = QJsonValue(rowid);
    data["lang"] = lang;
    data["name"] = name;
    data["content"] = content;
    data["md5_content"] = md5_content;

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// This handler will be delete classbook localization record
// *******************************************

CmdClassbookLocalizationDeleteRecordHandler::CmdClassbookLocalizationDeleteRecordHandler()
    : CmdHandlerBase("classbook_localization_delete_record", "Delete an article localization"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbook_localizationid").required().integer_().description("Localization id"));
}

// ---------------------------------------------------------------------

void CmdClassbookLocalizationDeleteRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    int classbook_localizationid = jsonRequest["classbook_localizationid"].toInt();
    query.prepare("SELECT id FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", jsonRequest["classbook_localizationid"].toInt());
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This localization doesn't exist"));
        return;
    }
    query.prepare("DELETE FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// This handler will be info classbook localization record
// *******************************************


CmdClassbookLocalizationInfoHandler::CmdClassbookLocalizationInfoHandler()
    : CmdHandlerBase("classbook_localization_info", "Find and display localization for an article by classbookid"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbook_localizationid").required().integer_().description("Localization id"));
}

// ---------------------------------------------------------------------

void CmdClassbookLocalizationInfoHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QJsonObject data;

    QSqlQuery query(db);
    int classbook_localizationid = jsonRequest["classbook_localizationid"].toInt();
    query.prepare("SELECT id FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This localization doesn't exist"));
        return;
    }

    query.prepare("SELECT classbookid, lang, name, content FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    query.next();
    QSqlRecord record = query.record();
    data["classbookid"] = record.value("classbookid").toInt();
    data["classbook_localizationid"] = classbook_localizationid;
    data["lang"] = record.value("lang").toString();
    data["name"] = record.value("name").toString();
    data["content"] = record.value("content").toString();


    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// This handler will be update classbook localization record
// *******************************************

CmdClassbookLocalizationUpdateRecordHandler::CmdClassbookLocalizationUpdateRecordHandler()
    : CmdHandlerBase("classbook_localization_update_record", "Update table with localization by classbookid"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbook_localizationid").required().integer_().description("Localization id"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("Article name"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("The content of the article"));
}

// ---------------------------------------------------------------------

void CmdClassbookLocalizationUpdateRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QJsonObject data;

    QSqlQuery query(db);
    int classbook_localizationid = jsonRequest["classbook_localizationid"].toInt();
    query.prepare("SELECT id FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This localization doesn't exist"));
        return;
    }

    QString name = jsonRequest["name"].toString().trimmed();
    QString content = jsonRequest["content"].toString().trimmed();

    //Set md5_content hash
    QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());

    query.prepare("UPDATE classbook_localization SET name = :name, content = :content, md5_content = :md5_content, updated = NOW() "
                  "WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    query.bindValue(":md5_content", md5_content);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    query.prepare("SELECT classbookid, lang FROM classbook_localization WHERE id=:id");
    query.bindValue(":id", classbook_localizationid);
    query.exec();
    query.next();
    QSqlRecord record = query.record();
    data["classbookid"] = record.value("classbookid").toInt();
    data["classbook_localizationid"] = classbook_localizationid;
    data["lang"] = record.value("lang").toString();
    data["name"] = name;
    data["content"] = content;
    data["md5_content"] = md5_content;

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


/*!
 * This handler will be add classbook proposal record
 * */

CmdClassbookProposalAddRecordHandler::CmdClassbookProposalAddRecordHandler()
    : CmdHandlerBase("classbook_proposal_add_record", "Propose an update of article"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("Classbookid for an article"));
    m_vInputs.push_back(CmdInputDef("lang").required().string_().description("Language"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("Article name"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("The content of the article"));
}

// ---------------------------------------------------------------------

void CmdClassbookProposalAddRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QJsonObject data;

    QSqlQuery query(db);
    int classbookid = jsonRequest["classbookid"].toInt();
    QString lang = jsonRequest["lang"].toString().trimmed();
    QString name = jsonRequest["name"].toString().trimmed();
    QString content = jsonRequest["content"].toString().trimmed();

    //obtain a current version of classbook name and content
    if(lang=="en"){
        query.prepare("SELECT name, content FROM classbook WHERE id = :classbookid");
        query.bindValue(":classbookid", classbookid);
    }
    else {
        query.prepare("SELECT name, content FROM classbook_localization WHERE lang = :lang");
        query.bindValue(":lang", lang);
    }
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This article or localization doesn't exist"));
        return;
    }
    QSqlRecord record = query.record();
    QString name_before = record.value("name").toString();
    QString content_before = record.value("content").toString();

    //Set md5_content hash
    QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());

    //generate uuid
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");

    query.prepare("INSERT INTO classbook_proposal("
                  "classbookid,"
                  "uuid,"
                  "lang,"
                  "name,"
                  "content,"
                  "name_before,"
                  "content_before,"
                  "md5_content,"
                  "created"
                  ") "
                  "VALUES("
                  ":classbookid,"
                  ":uuid,"
                  ":lang,"
                  ":name,"
                  ":content,"
                  ":name_before,"
                  ":content_before,"
                  ":md5_content,"
                  "NOW()"
                  ")");
    query.bindValue(":classbookid", classbookid);
    query.bindValue(":uuid", uuid);
    query.bindValue(":lang", lang);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    query.bindValue(":name_before", name_before);
    query.bindValue(":content_before", content_before);
    query.bindValue(":md5_content", md5_content);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    int rowid = query.lastInsertId().toInt();
    data["classbookid"] = classbookid;
    data["classbook_proposal_id"] = QJsonValue(rowid);
    data["lang"] = lang;
    data["name"] = name;
    data["content"] = content;
    data["name_before"] = name_before;
    data["content_before"] = content_before;
    data["md5_content"] = md5_content;

    jsonResponse["data"] = data;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be delete classbook proposal record
 * */

CmdClassbookProposalDeleteRecordHandler::CmdClassbookProposalDeleteRecordHandler()
    : CmdHandlerBase("classbook_proposal_delete_record", "Delete a proposal of updating an article"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbook_proposal_id").required().integer_().description("Proposal id"));
}

// ---------------------------------------------------------------------

void CmdClassbookProposalDeleteRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    int classbook_proposal_id = jsonRequest["classbook_proposal_id"].toInt();
    query.prepare("SELECT id FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", jsonRequest["classbook_proposal_id"].toInt());
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This proposal doesn't exist"));
        return;
    }
    query.prepare("DELETE FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be info classbook proposal record
 * */

CmdClassbookProposalInfoHandler::CmdClassbookProposalInfoHandler()
    : CmdHandlerBase("classbook_proposal_info", "Find and display all proposal data by id"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbook_proposal_id").required().integer_().description("Proposal id"));
}

// ---------------------------------------------------------------------

void CmdClassbookProposalInfoHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QJsonObject data;

    QSqlQuery query(db);
    int classbook_proposal_id = jsonRequest["classbook_proposal_id"].toInt();
    query.prepare("SELECT id FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", jsonRequest["classbook_proposal_id"].toInt());
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This proposal doesn't exist"));
        return;
    }

    query.prepare("SELECT classbookid, lang, name, content FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    query.next();
    QSqlRecord record = query.record();
    data["classbookid"] = record.value("classbookid").toInt();
    data["id"] = classbook_proposal_id;
    data["lang"] = record.value("lang").toString();
    data["name"] = record.value("name").toString();
    data["content"] = record.value("content").toString();


    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be list classbook proposal record
 * */

CmdClassbookProposalListHandler::CmdClassbookProposalListHandler()
    : CmdHandlerBase("classbook_proposal_list", "Display list of proposals by classbookid"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbookid").optional().integer_().description("Classbookid for an article"));
    m_vInputs.push_back(CmdInputDef("lang").optional().string_().description("Language"));
}

// ---------------------------------------------------------------------

void CmdClassbookProposalListHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);

    QJsonObject item;
    QJsonArray data;

    QString sQuery;
    QMap<QString, QJsonValue> mapFilter;

    //checkout and validation of classbookid
    if(jsonRequest.contains("classbookid")){
        query.prepare("SELECT id FROM classbook WHERE id = :classbookid");
        query.bindValue(":classbookid", jsonRequest["classbookid"].toInt());
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if(!query.next()){
            pRequest->sendMessageError(cmd(), Error(404, "This article doesn't exist"));
            return;
        }
        mapFilter.insert("classbookid", jsonRequest["classbookid"].toInt());
    }

    //checkout of lang and generation of query's bone
    if(jsonRequest.contains("lang")){
        mapFilter.insert("lang", jsonRequest["lang"].toString().trimmed());
        sQuery = "SELECT id, name FROM classbook_proposal";
    }
    else sQuery = "SELECT id, name, lang FROM classbook_proposal";

    //generation of the rest of the query
    if(mapFilter.size() > 0) sQuery += " WHERE ";
    bool bFirst = true;
    foreach(QString key, mapFilter.keys()){
        if(!bFirst) sQuery += " AND ";
        bFirst = false;
        sQuery +=  key + " = :" + key;
    }
    query.prepare(sQuery);

    //binding of values
    foreach(QString key, mapFilter.keys()){
        QMap<QString, QJsonValue>::const_iterator v = mapFilter.lowerBound(key);
        if(key=="classbookid")
            query.bindValue(":" + key, v.value().toInt());
        else
            query.bindValue(":" + key, v.value());
    }
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        item["id"] = record.value("id").toInt();
        item["classbookid"] = jsonRequest["classbookid"].toInt();
        if(jsonRequest.contains("lang")){
            item["lang"] = jsonRequest["lang"].toString().trimmed();
        }
        else item["lang"] = record.value("lang").toString().trimmed();
        item["name"] = record.value("name").toString();
        data.push_back(item);
    }

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be prepare classbook proposal record
 * */
CmdClassbookProposalPrepareMergeRecordHandler::CmdClassbookProposalPrepareMergeRecordHandler()
    : CmdHandlerBase("classbook_propasal_prepare_merge_record", "Prepare to merge updating requests"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbook_proposal_id").required().integer_().description("Proposal id"));
}

// ---------------------------------------------------------------------

void CmdClassbookProposalPrepareMergeRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QJsonObject data;

    QSqlQuery query(db);
    QSqlRecord record = query.record();
    int classbook_proposal_id = jsonRequest["classbook_proposal_id"].toInt();
    query.prepare("SELECT id FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", jsonRequest["classbook_proposal_id"].toInt());
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This proposal doesn't exist"));
        return;
    }

    query.prepare("SELECT content FROM classbook WHERE id IN (SELECT classbookid FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    QString curtxt = record.value("content").toString();

    query.prepare("SELECT content, content_before FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    QString txt1 = record.value("content").toString();
    QString txt2 = record.value("content_before").toString();
    std::vector<row *> arr1, arr2;
    UtilsMergeText::merge(curtxt, txt1, txt2, arr1, arr2);

    //TO DO final merge, lang checkout, update output (with data)

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be return classbook content (duplicate handler ? )
 * */

CmdClassbookHandler::CmdClassbookHandler()
    : CmdHandlerBase("classbook", "Return classbook contents"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

}

// ---------------------------------------------------------------------

void CmdClassbookHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QJsonArray contents;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM classbook ORDER BY id ASC");
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        QString uuid = record.value("uuid").toString();
        QJsonObject jsonItem;
        jsonItem["id"] = record.value("id").toInt();
        jsonItem["parentid"] = record.value("parentid").toInt();
        jsonItem["uuid"] = uuid;
        jsonItem["parentuuid"] = record.value("parentuuid").toString();
        jsonItem["name_ru"] = record.value("name_ru").toString();
        jsonItem["name_en"] = record.value("name_en").toString();
        jsonItem["name"] = record.value("name_en").toString();
        jsonItem["link"] = QJsonValue("files/classbook/" + uuid + "_en.md");
        contents.push_back(jsonItem);
    }

    jsonResponse["items"] = contents;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
