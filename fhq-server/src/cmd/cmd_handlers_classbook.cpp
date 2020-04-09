#include <cmd_handlers_classbook.h>
#include <runtasks.h>
#include <iostream>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>
#include <quazip.h>
#include <quazipfile.h>
#include <quazipfileinfo.h>
#include <wsjcpp_diff_text.h>
#include <validators.h>
#include <fallen.h>
#include <wsjcpp_core.h>
#include <wsjcpp_hashes.h>

// *******************************************
// * This handler will be add classbook record
// *******************************************

CmdClassbookAddRecordHandler::CmdClassbookAddRecordHandler()
    : CmdHandlerBase("classbook_add_record", "Adds a new article with the specified name, content, and id.") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("parentid", "pareintid for classbook article");
    requireStringParam("name", "name of article");
    requireStringParam("content", "content of article");
    optionalStringParam("uuid", "uuid of article")
        .addValidator(new WsjcppValidatorUUID());
    optionalIntegerParam("ordered", "order of article");
}

// ---------------------------------------------------------------------

void CmdClassbookAddRecordHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();

    QSqlDatabase db = *(pDatabase->database());

    int nParentID = pRequest->getInputInteger("parentid", 0);

    // check parentid in database
    QSqlQuery query(db);
    if (nParentID != 0) {
        query.prepare("SELECT name FROM classbook WHERE id = :parentid");
        query.bindValue(":parentid", nParentID);
        query.exec();
        if (!query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found article with this id"));
            return;
        }
    }

    std::string sName = pRequest->getInputString("name", "");
    std::string sContent = pRequest->getInputString("content", "");

    //Set uuid from request if available, else generate uuid
    std::string sUuid = pRequest->getInputString("uuid", "");
    if (sUuid != "") {
        query.prepare("SELECT uuid FROM classbook WHERE uuid = :uuid");
        query.bindValue(":uuid", QString::fromStdString(sUuid));
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(403, "Uuid already exist"));
            return;
        }
    } else {
        sUuid = WsjcppCore::createUuid();
    }

    //Set md5_content hash
    std::string sContentMd5 = WsjcppHashes::md5_calc_hex(sContent);

    //Find parentuuid from database
    QString parentuuid = "00000000-0000-0000-0000-000000000000";
    if (nParentID != 0) {
        query.prepare("SELECT uuid FROM classbook WHERE id = :parentid");
        query.bindValue(":parentid", nParentID);
        query.exec(); // TODO check error
        query.next();
        QSqlRecord record = query.record();
        parentuuid = record.value("uuid").toString();
    }

    //Set ordered of article: increment max of child's ordered
    int nOrdered;
    if (jsonRequest.find("ordered") != jsonRequest.end()) {
        nOrdered = jsonRequest.at("ordered").get<int>();
    } else {
        query.prepare("SELECT MAX(ordered) AS max FROM classbook WHERE parentid=:parentid");
        query.bindValue(":parentid", nParentID);
        query.exec(); // TODO check errors
        QSqlRecord record = query.record();
        if (!record.value("max").isNull()) {
            nOrdered = record.value("max").toInt() + 1;
        } else {
            if (nParentID != 0) {
                query.prepare("SELECT ordered FROM classbook WHERE id=:parentid");
                query.bindValue(":parentid", nParentID);
                query.exec(); // TODO check errors
                if (query.next()) {
                    QSqlRecord record = query.record();
                    nOrdered = record.value("ordered").toInt() + 1;
                } else {
                    pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                    return;
                }
            } else {
                nOrdered = 1;
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
    query.bindValue(":parentid", nParentID);
    query.bindValue(":ordered", nOrdered);
    query.bindValue(":uuid", QString::fromStdString(sUuid));
    query.bindValue(":parentuuid", parentuuid);
    query.bindValue(":name", QString::fromStdString(sName));
    query.bindValue(":content", QString::fromStdString(sContent));
    query.bindValue(":md5_content", QString::fromStdString(sContentMd5));
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonResponse;
    nlohmann::json jsonData;
    int nRowId = query.lastInsertId().toInt();
    jsonData["classbookid"] = nRowId;
    jsonData["parentid"] = nParentID;
    jsonData["name"] = sName;
    jsonData["content"] = sContent;
    jsonData["md5_content"] = sContentMd5;
    // TODO sUuid

    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// * This handler will be delete classbook record
// *******************************************

CmdClassbookDeleteRecordHandler::CmdClassbookDeleteRecordHandler()
    : CmdHandlerBase("classbook_delete_record", "Delete a article with a given classbookid") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbookid", "id for classbook article");
}

// ---------------------------------------------------------------------

void CmdClassbookDeleteRecordHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();
    

    QSqlDatabase db = *(pDatabase->database());

    int nClassbookID = 0;
    if (jsonRequest.find("classbookid") != jsonRequest.end()) {
        nClassbookID = jsonRequest.at("classbookid").get<int>();
    }

    // DELETE Record IF haven't childs
    QSqlQuery query(db);
    if (nClassbookID !=0) {
        query.prepare("SELECT id FROM classbook WHERE parentid=:classbookid");
        query.bindValue(":classbookid", nClassbookID);
        query.exec(); // TODO check db error
        if (query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(403, "Could not delete, because childs exists. Please remove childs first."));
            return;
        }
        //Delete record in classbook
        query.prepare("DELETE FROM classbook WHERE id=:classbookid");
        query.bindValue(":classbookid", nClassbookID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }

        //Delete record's localization
        query.prepare("DELETE FROM classbook_localization WHERE classbookid=:classbookid");
        query.bindValue(":classbookid", nClassbookID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    nlohmann::json jsonResponse;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// * This handler will be export classbook record
// *******************************************

CmdClassbookExportHandler::CmdClassbookExportHandler()
    : CmdHandlerBase("classbook_export", "Export classbook's articles to html or markdown, optionally in zip archive.") {

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireStringParam("output", "The output file format");
    requireStringParam("lang", "The output file format");
    optionalBooleanParam("zip", "Zipping the output");
}

// ---------------------------------------------------------------------

void CmdClassbookExportHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    std::string sLang = "";
    if (jsonRequest.find("lang") != jsonRequest.end()) {
        sLang = jsonRequest["lang"];
    }

    std::string sOutput = "";
    if (jsonRequest.find("output") != jsonRequest.end()) {
        sOutput = jsonRequest["output"];
    }

    //Check parametrs
    if (sOutput != "html" && sOutput != "markdown") {
        pRequest->sendMessageError(cmd(), WsjcppError(403, "The output is not supported."));
        return;
    }
    QList<QString> langs;
    langs << "en" << "de" << "ru"; // TODO move to basic support employ
    if (!langs.contains(QString::fromStdString(sLang))) {
        pRequest->sendMessageError(cmd(), WsjcppError(403, "The language is not supported."));
        return;
    }

    QString tmpFileDir = QDir::tempPath();
    QString tmpFile = tmpFileDir + "/freehackquest-classbook_export_file";
    QFile file(tmpFile);
    file.open(QIODevice::WriteOnly);
    if (sOutput == "html") {
        createHtml(&file, sLang, query);
    }
    if (sOutput == "markdown") {
        createMD(&file, sLang, query);
    }
    file.close();
    file.open(QIODevice::ReadOnly);

    if (jsonRequest.find("zip") != jsonRequest.end()) {
        bool bZip = jsonRequest["zip"];
        if (bZip) {
            QString tmpDir = QDir::tempPath();
            QString tmpZipFile = tmpDir + "/freehackquest-classbook_" + QString::fromStdString(sLang) + ".zip";
            // prepare zip archive
            QuaZip zip(tmpZipFile);
            zip.open(QuaZip::mdCreate);
            QuaZipFile export_zipfile(&zip);

            QString name = "freehackquest-classbook." + QString::fromStdString(sOutput);
            export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(name));
            // After .toString(), you should specify a text codec to use to encode the
            // string data into the (binary) file. Here, I use UTF-8:
            export_zipfile.write(file.readAll());
            export_zipfile.close();
            zip.close();
            QFile fileZip(tmpZipFile);
            if (!fileZip.open(QIODevice::ReadOnly)) {
                pRequest->sendMessageError(cmd(), WsjcppError(500, "Could not open zip file"));
                return;
            }
            QByteArray baZip = fileZip.readAll();
            nlohmann::json jsonData;
            jsonData["zipfile_base64"] = QString(baZip.toBase64()).toStdString();
            jsonData["zipfile_name"] = name.toStdString();
            jsonResponse["data"] = jsonData;
            fileZip.close();
            fileZip.remove();
        }
    } else {
        jsonResponse["data"] = QString::fromUtf8(file.readAll()).toStdString();
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
    file.close();
    file.remove();
}

// ---------------------------------------------------------------------

void CmdClassbookExportHandler::createHtml(QFile *file, const std::string &sLang, QSqlQuery query) {
    QTextStream out(file);
    out.setCodec("UTF-8");
    QMap <int, QString> name_of_articles;
    if (sLang == "en") {
        query.prepare("SELECT id, name FROM classbook ORDER BY ordered");
        query.exec(); // TODO check errors
        while (query.next()) {
            QSqlRecord record = query.record();
            int id = record.value("id").toInt();
            name_of_articles[id] = record.value("name").toString();
        }
    } else {
        query.prepare("SELECT classbookid, name FROM classbook_localization WHERE lang=:lang");
        query.bindValue(":lang", QString::fromStdString(sLang));
        query.exec(); // TODO check errors
        while (query.next()) {
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
    for (i = name_of_articles.begin(); i != name_of_articles.end(); ++i) {
        out << "<h3><a href=#" << i.key() << ">" << i.value().toUtf8() << "</a></h3>" << endl;
    }

    if (sLang == "en") {
        query.prepare("SELECT id, name, content FROM classbook ORDER BY ordered");
        query.exec(); // TODO check errors
        while (query.next()) {
            QSqlRecord record = query.record();
            out << "<article id=" << record.value("id").toInt() << "><h2>";
            out << record.value("name").toString().toUtf8() << "</h2>" << endl;
            out << "<p>" << record.value("content").toString().toUtf8() << "</p></article>" << endl;
        }
    } else {
        query.prepare("SELECT classbookid, name, content FROM classbook_localization WHERE lang=:lang");
        query.bindValue(":lang", QString::fromStdString(sLang));
        query.exec(); // TODO check errors
        while (query.next()) {
            QSqlRecord record = query.record();
            out << "<article><h2 id =" << record.value("classbookid").toInt() << ">";
            out << record.value("name").toString().toUtf8() << "</h2>" << endl;
            out << "<p>" << record.value("content").toString().toUtf8() << "</p></article>" << endl;
        }
    }
    out << "</body></html>" << endl;
}

// ---------------------------------------------------------------------

void CmdClassbookExportHandler::createMD(QFile *file, const std::string &sLang, QSqlQuery query) {
    QTextStream out(file);
    out.setCodec("UTF-8");
    QList<QString> name_of_articles;
    if (sLang == "en") {
        query.prepare("SELECT name FROM classbook ORDER BY ordered");
        query.exec(); // TODO check errors
        while (query.next()) {
            QSqlRecord record = query.record();
            name_of_articles << record.value("name").toString();
        }
    } else {
        query.prepare("SELECT name FROM classbook_localization WHERE lang=:lang");
        query.bindValue(":lang", QString::fromStdString(sLang));
        query.exec(); // TODO check errors
        while (query.next()) {
            QSqlRecord record = query.record();
            name_of_articles << record.value("name").toString();
        }
    }

    out << "# Freehackquest classbook" << endl;
    out << "## Table of content" << endl;
    for (int i = 0; i < name_of_articles.size(); ++i) {
        out << "* " << name_of_articles.at(i).toUtf8() << endl;
    }

    if (sLang == "en") {
        query.prepare("SELECT name, content FROM classbook ORDER BY ordered");
        query.exec(); // TODO check errors
        while (query.next()) {
            QSqlRecord record = query.record();
            out << "### " << record.value("name").toString().toUtf8() << endl;
            out << record.value("content").toString().toUtf8() << endl;
        }
    } else {
        query.prepare("SELECT name, content FROM classbook_localization WHERE lang=:lang");
        query.bindValue(":lang", QString::fromStdString(sLang));
        query.exec(); // TODO check errors
        while (query.next()) {
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
    : CmdHandlerBase("classbook_info", "Return name and content, langs, path classbook article with a given id") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbookid", "id for the classbook article");
    optionalStringParam("lang", "Set lang for the article"); // TODO validator lang
    
}

// ---------------------------------------------------------------------

void CmdClassbookInfoHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    QSqlDatabase db = *(pDatabase->database());

    int nClassbookID = 0;
    if (jsonRequest.find("classbookid") != jsonRequest.end()) {
        nClassbookID = jsonRequest.at("classbookid").get<int>();
    }

    QSqlQuery query(db);
    nlohmann::json jsonInfo;

    // GET parentid and uuid for the article
    query.prepare("SELECT parentid, uuid FROM classbook WHERE id = :classbookid");
    query.bindValue(":classbookid", nClassbookID);
    query.exec();
    if (query.next()) {
        QSqlRecord record = query.record();
        jsonInfo["classbookid"] = nClassbookID;
        jsonInfo["parentid"] = record.value("parentid").toInt();
        jsonInfo["uuid"] = record.value("uuid").toString().toStdString();
    } else {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found the article"));
        return;
    }

    //SET lang
    std::string sLang;
    if (jsonRequest.find("lang") != jsonRequest.end()) {
        sLang = jsonRequest["lang"];
        QList<QString> allow_lang = {"en", "ru","de"};
        if (!allow_lang.contains(QString::fromStdString(sLang))) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Language is not support"));
            return;
        }
    } else {
        sLang = "en";
    }

    //GET localization
    if (sLang != "en") {
        //GET localization for the article with a given lang
        sLang = jsonRequest["lang"];
        query.prepare("SELECT name, content FROM classbook_localization WHERE classbookid=:classbookid AND lang=:lang");
        query.bindValue(":classbookid", nClassbookID);
        query.bindValue(":lang", QString::fromStdString(sLang));
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonInfo["lang"] = sLang;
            jsonInfo["name"] = record.value("name").toString().toStdString();
            jsonInfo["content"] = record.value("content").toString().toStdString();
        } else {
            //GET default localization for the article
            query.prepare("SELECT name, content, ordered FROM classbook WHERE id=:classbookid");
            query.bindValue(":classbookid", nClassbookID);
            query.exec();
            if (query.next()) {
                QSqlRecord record = query.record();
                jsonInfo["lang"] = "en";
                jsonInfo["name"] = record.value("name").toString().toStdString();
                jsonInfo["content"] = record.value("content").toString().toStdString();
                jsonInfo["ordered"] = record.value("ordered").toInt();
            } else {
                pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found the article"));
                return;
            }
        }
    } else {
        // GET default localization for the article
        query.prepare("SELECT name, content, ordered FROM classbook WHERE id=:classbookid");
        query.bindValue(":classbookid", nClassbookID);
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonInfo["lang"] = sLang;
            jsonInfo["name"] = record.value("name").toString().toStdString();
            jsonInfo["content"] = record.value("content").toString().toStdString();
            jsonInfo["ordered"] = record.value("ordered").toInt();
        } else {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found the article"));
            return;
        }
    }

    // FIND langs for the article
    nlohmann::json jsonLangs;
    query.prepare("SELECT id, lang FROM classbook_localization WHERE classbookid=:classbookid");
    query.bindValue(":classbookid", nClassbookID);
    query.exec();
    if (query.next()) {
        QSqlRecord record = query.record();
        std::string local_lang = record.value("lang").toString().toStdString();
        jsonLangs[local_lang] = record.value("id").toInt();
    }
    jsonInfo["langs"] = jsonLangs;

    //FIND parents for the article
    nlohmann::json jsonParents = nlohmann::json::array();
    QSet<int> set_of_parent;
    int nParentId = jsonInfo["parentid"];
    for (int i = 0; i < 5; ++i) {
        //END IT root article
        if (nParentId==0) {
            nlohmann::json jsonParent;
            jsonParent["classbookid"] = 0;
            jsonParent["parentid"] = 0;
            jsonParent["name"] = "Root";
            jsonParents.push_back(jsonParent);
            break;
        }
        //CONTINUE if already have a article in parents
        if (set_of_parent.contains(nParentId)) {
            continue;
        }
        query.prepare("SELECT id, name, parentid FROM classbook WHERE id=:parentid");
        query.bindValue(":parentid", nParentId);
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            nlohmann::json jsonParent;
            int nClassBookId_ = record.value("id").toInt();
            jsonParent["classbookid"] = nClassBookId_;
            if (nClassbookID == nClassBookId_) {
                continue;
            }
            nParentId = record.value("parentid").toInt();
            jsonParent["parentid"] = nParentId;
            jsonParent["name"] = record.value("name").toString().toStdString();
            jsonParents.push_back(jsonParent);
            set_of_parent.insert(nClassBookId_);
        } else {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Error in PATHFINDER. Not found the article with a given classbookid"));
            return;
        }
    }
    //ADD parents to response
    jsonInfo["parents"] = jsonParents;

    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonInfo;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// * This handler will be return classbook records list
// *******************************************


CmdClassbookListHandler::CmdClassbookListHandler()
    : CmdHandlerBase("classbook_list", "Return list of classbook articles") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    requireIntegerParam("parentid", "parentid for classbook articles"); // TODO validator id
    optionalStringParam("lang", "lang for classbook articles")
        .addValidator(new ValidatorLanguage());
    optionalStringParam("search", "Search string for classbook articles");
}

// ---------------------------------------------------------------------

void CmdClassbookListHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse;
    nlohmann::json jsonData = nlohmann::json::array();
    

    int nParentID = pRequest->getInputInteger("parentid", 0);
    std::string sLang = pRequest->getInputString("lang", "en");
    
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    if (jsonRequest.find("search") != jsonRequest.end()) {
        std::string sSearch = jsonRequest["search"];

        QSqlQuery query1(db);
        if (sLang == "en") {
            query1.prepare("SELECT id, name FROM classbook WHERE name LIKE :search OR content LIKE :search ORDER BY ordered;");
            query1.bindValue(":search", QString::fromStdString("%" + sSearch + "%"));
        } else {
            query1.prepare("SELECT id, name FROM classbook_localization WHERE name LIKE :search OR content LIKE :search");
            query1.bindValue(":search", QString::fromStdString("%" + sSearch + "%"));
        }

        if (!query1.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query1.lastError().text().toStdString()));
            return;
        }
        while (query1.next()) {
            QSqlRecord record = query1.record();
            nlohmann::json jsonItem;
            
            int classbookid;
            jsonItem["parentid"] = nParentID;
            classbookid = record.value("id").toInt();
            jsonItem["classbookid"] = classbookid;
            if (sLang != "en") {
                query.prepare("SELECT classbookid, name FROM classbook_localization "
                                  "WHERE lang=:lang AND (name LIKE '%:search%' OR content LIKE '%:search%')");
                query.bindValue(":search", QString::fromStdString(sSearch));
                query.bindValue(":lang", QString::fromStdString(sLang));
            }
            jsonItem["name"] = record.value("name").toString().toStdString();  // TODO escaping html chars

            //COUNT childs for an article
            QSqlQuery query_childs(db);
            query_childs.prepare("SELECT COUNT(id) AS childs FROM classbook WHERE parentid =:classbookid");
            query_childs.bindValue(":classbookid", classbookid);
            query_childs.exec();
            int childs = 0;
            if (query_childs.next()) {
                QSqlRecord record_childs = query_childs.record();
                childs = record_childs.value("childs").toInt();
            }
            jsonItem["childs"] = childs;

            //COUNT proposals for an article
            QSqlQuery query_proposals(db);
            query_proposals.prepare("SELECT COUNT(id) AS proposals FROM classbook_proposal "
                                    "WHERE classbookid = :classbookid AND lang = :lang");
            query_proposals.bindValue(":classbookid", classbookid);
            query_proposals.bindValue(":lang", QString::fromStdString(sLang));
            query_proposals.exec(); // TODO check errors
            int proposals = 0;
            if (query_proposals.next()) {
                QSqlRecord record_proposals = query_proposals.record();
                proposals = record_proposals.value("proposals").toInt();
            }
            jsonItem["proposals"] = proposals;

            jsonData.push_back(jsonItem);
        }

    } else {
        //CHECK exist parentid in DB
        query.prepare("SELECT name FROM classbook WHERE id =:parentid");
        query.bindValue(":parentid", nParentID);
        query.exec(); // TODO check errors
        if (!query.next() && nParentID != 0) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found the article with a given parentid"));
            return;
        }

        query.prepare("SELECT id, name FROM classbook WHERE parentid =:parentid ORDER BY ordered");
        query.bindValue(":parentid", nParentID);
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            nlohmann::json jsonItem;
            int classbookid;
            jsonItem["parentid"] = nParentID;
            classbookid = record.value("id").toInt();
            jsonItem["classbookid"] = classbookid;

            //GET name with the lang
            if (sLang == "en") {
                jsonItem["name"] = record.value("name").toString().toStdString(); // TODO escaping html chars
            } else {
                QSqlQuery query_lang(db);
                query_lang.prepare("SELECT name FROM classbook_localization WHERE classbookid=:classbookid AND lang=:lang");
                query_lang.bindValue(":classbookid", classbookid);
                query_lang.bindValue(":lang", QString::fromStdString(sLang));
                query_lang.exec(); // TODO check errors
                if (query_lang.next()) {
                    QSqlRecord record_lang = query_lang.record();
                    jsonItem["name"] = record_lang.value("name").toString().toStdString();
                } else {
                    jsonItem["name"] = record.value("name").toString().toStdString();
                }
            }

            //COUNT childs for an article
            QSqlQuery query_childs(db);
            query_childs.prepare("SELECT COUNT(id) AS childs FROM classbook WHERE parentid =:classbookid");
            query_childs.bindValue(":classbookid", classbookid);
            query_childs.exec(); // TODO check errors
            int childs = 0;
            if (query_childs.next()) {
                QSqlRecord record_childs = query_childs.record();
                childs = record_childs.value("childs").toInt();
            }
            jsonItem["childs"] = childs;

            //COUNT proposals for an article
            QSqlQuery query_proposals(db);
            query_proposals.prepare("SELECT COUNT(id) AS proposals FROM classbook_proposal "
                                    "WHERE classbookid =:classbookid AND lang=:lang");
            query_proposals.bindValue(":classbookid", classbookid);
            query_proposals.bindValue(":lang", QString::fromStdString(sLang));
            query_proposals.exec(); // TODO check errors
            int proposals = 0;
            if (query_proposals.next()) {
                QSqlRecord record_proposals = query_proposals.record();
                proposals = record_proposals.value("proposals").toInt();
            }
            jsonItem["proposals"] = proposals;

            jsonData.push_back(jsonItem);
        }
    }

    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// * This handler will be update classbook record
// *******************************************

CmdClassbookUpdateRecordHandler::CmdClassbookUpdateRecordHandler()
    : CmdHandlerBase("classbook_update_record", "Update a article with a given classbookid") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbookid", "id for classbook article");
    optionalStringParam("name", "name for classbook article");
    optionalStringParam("content", "content for classbook article");
    optionalIntegerParam("ordered", "ordered for classbook article");
    optionalIntegerParam("parentid", "parentid for classbook article");
}

// ---------------------------------------------------------------------

void CmdClassbookUpdateRecordHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    int nClassbookID = pRequest->getInputInteger("classbookid", 0);

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    //IF classbookid = 0, THEN reject request
    if (nClassbookID == 0) {
        pRequest->sendMessageError(cmd(), WsjcppError(403, "Not today. It's root article id"));
        return;
    }

    //Find article with a given classbookid
    QSqlQuery query(db);
    query.prepare("SELECT name FROM classbook WHERE id = :classbookid");
    query.bindValue(":classbookid", nClassbookID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found a article with a given classbookid"));
        return;
    }

    //CHECK Do we have anything to change?
    if (jsonRequest.find("name") == jsonRequest.end()
        || jsonRequest.find("content") == jsonRequest.end()
        || jsonRequest.find("ordered") == jsonRequest.end()
        || jsonRequest.find("parentid") == jsonRequest.end()
    ) {
        pRequest->sendMessageError(cmd(), WsjcppError(403, "Not found a charges. Not enough parameters"));
        return;
    }

    int nParentID;
    //FIND article with id = parentid AND UPDATE parentid IF exist
    if (jsonRequest.find("parentid") != jsonRequest.end()) {
        nParentID = jsonRequest.at("parentid").get<int>();

        if (nParentID != 0) {
            //CHECK existence of the article
            query.prepare("SELECT name FROM classbook WHERE id=:parentid");
            query.bindValue(":parentid", nParentID);
            if (!query.exec()) {
                 pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
                return;
            }
            if (!query.next()) {
                pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found a article with a given parentid"));
                return;
            }
        }

        query.prepare("UPDATE classbook SET parentid=:parentid WHERE id=:classbookid");
        query.bindValue(":classbookid", nClassbookID);
        query.bindValue(":parentid", nParentID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    //UPDATE name for article
    if (jsonRequest.find("name") != jsonRequest.end()) {
        std::string sName = jsonRequest["name"];
        query.prepare("UPDATE classbook SET name=:name WHERE id = :classbookid");
        query.bindValue(":classbookid", nClassbookID);
        query.bindValue(":name", QString::fromStdString(sName));
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    //UPDATE content for article
    if (jsonRequest.find("content") != jsonRequest.end()) {
        std::string sContent = jsonRequest["content"];
        std::string sContentMd5_ = WsjcppHashes::md5_calc_hex(sContent);
        query.prepare("UPDATE classbook SET content = :content, md5_content = :md5_content WHERE id = :classbookid");
        query.bindValue(":classbookid", nClassbookID);
        query.bindValue(":content", QString::fromStdString(sContent));
        query.bindValue(":md5_content", QString::fromStdString(sContentMd5_));
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    //UPDATE ordered for article
    if (jsonRequest.find("ordered") != jsonRequest.end()) {
        int ordered = jsonRequest.at("ordered").get<int>();
        query.prepare("UPDATE classbook SET ordered=:ordered WHERE id = :classbookid");
        query.bindValue(":classbookid", nClassbookID);
        query.bindValue(":ordered", ordered);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    //UPDATE updated column
    query.prepare("UPDATE classbook SET updated = NOW() WHERE id = :classbookid");
    query.bindValue(":classbookid", nClassbookID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    //GET article info
    nlohmann::json jsonInfo;
    query.prepare("SELECT id, name, content, md5_content, ordered, parentid FROM classbook WHERE id = :classbookid");
    query.bindValue(":classbookid", nClassbookID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (query.next()) {
        QSqlRecord record = query.record();
        jsonInfo["classbookid"] = nClassbookID;
        jsonInfo["parentid"] = record.value("parentid").toInt();
        jsonInfo["name"] = record.value("name").toString().toStdString();
        jsonInfo["content"] = record.value("content").toString().toStdString();
        jsonInfo["md5_content"] = record.value("md5_content").toString().toStdString();
        jsonInfo["ordered"] = record.value("ordered").toInt();
    } else {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found article"));
        return;
    }

    jsonResponse["data"] = jsonInfo;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// * This handler will be add classbook localization record
// *******************************************


CmdClassbookLocalizationAddRecordHandler::CmdClassbookLocalizationAddRecordHandler()
    : CmdHandlerBase("classbook_localization_add_record", "Add a new article localization for the English version") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbookid", "Classbookid for article localization");
    requireStringParam("lang", "Language"); // TODO validator lang
    requireStringParam("name", "Article name");
    requireStringParam("content", "The content of the article");
}

// ---------------------------------------------------------------------

void CmdClassbookLocalizationAddRecordHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse;

    int nClassbookID = -1;
    if (jsonRequest.find("classbookid") != jsonRequest.end()) {
        nClassbookID = jsonRequest.at("classbookid").get<int>();
    }

    std::string sLang = "";
    if (jsonRequest.find("lang") != jsonRequest.end()) {
        sLang = jsonRequest["lang"];
    }

    std::string sName = "";
    if (jsonRequest.find("name") != jsonRequest.end()) {
        sName = jsonRequest["name"];
    }

    std::string sContent = "";
    if (jsonRequest.find("content") != jsonRequest.end()) {
        sContent = jsonRequest["content"];
    }

    QSqlDatabase db = *(pDatabase->database());

    nlohmann::json jsonData;

    QSqlQuery query(db);
    query.prepare("SELECT lang FROM classbook_localization WHERE lang = :lang AND classbookid=:classbookid");
    query.bindValue(":lang", QString::fromStdString(sLang));
    query.bindValue(":classbookid", nClassbookID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "This lang already exist"));
        return;
    }

    //Set md5_content hash
    std::string sContentMd5_ = WsjcppHashes::md5_calc_hex(sContent);

    //generate uuid
    std::string sUuid = WsjcppCore::createUuid();

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
    query.bindValue(":classbookid", nClassbookID);
    query.bindValue(":uuid", QString::fromStdString(sUuid));
    query.bindValue(":lang", QString::fromStdString(sLang));
    query.bindValue(":name", QString::fromStdString(sName));
    query.bindValue(":content", QString::fromStdString(sContent));
    query.bindValue(":md5_content", QString::fromStdString(sContentMd5_));
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    int rowid = query.lastInsertId().toInt();
    jsonData["classbookid"] = nClassbookID;
    jsonData["classbook_localizationid"] = rowid;
    jsonData["lang"] = sLang;
    jsonData["name"] = sName;
    jsonData["content"] = sContent;
    jsonData["md5_content"] = sContentMd5_;

    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// This handler will be delete classbook localization record
// *******************************************

CmdClassbookLocalizationDeleteRecordHandler::CmdClassbookLocalizationDeleteRecordHandler()
    : CmdHandlerBase("classbook_localization_delete_record", "Delete an article localization") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbook_localizationid", "Localization id");
}

// ---------------------------------------------------------------------

void CmdClassbookLocalizationDeleteRecordHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    int nClassbookLocalizationID = -1;
    if (jsonRequest.find("classbook_localizationid") != jsonRequest.end()) {
        nClassbookLocalizationID = jsonRequest.at("classbook_localizationid").get<int>();
    }

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT id FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", nClassbookLocalizationID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This localization doesn't exist"));
        return;
    }
    query.prepare("DELETE FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", nClassbookLocalizationID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// This handler will be info classbook localization record
// *******************************************


CmdClassbookLocalizationInfoHandler::CmdClassbookLocalizationInfoHandler()
    : CmdHandlerBase("classbook_localization_info", "Find and display localization for an article by classbookid") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbook_localizationid", "Localization id");
}

// ---------------------------------------------------------------------

void CmdClassbookLocalizationInfoHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    int nClassbookLocalizationID = -1;
    if (jsonRequest.find("classbook_localizationid") != jsonRequest.end()) {
        nClassbookLocalizationID = jsonRequest.at("classbook_localizationid").get<int>();
    }

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    nlohmann::json jsonData;

    QSqlQuery query(db);
    query.prepare("SELECT id FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", nClassbookLocalizationID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This localization doesn't exist"));
        return;
    }

    query.prepare("SELECT classbookid, lang, name, content FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", nClassbookLocalizationID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    query.next();
    QSqlRecord record = query.record();
    jsonData["classbookid"] = record.value("classbookid").toInt();
    jsonData["classbook_localizationid"] = nClassbookLocalizationID;
    jsonData["lang"] = record.value("lang").toString().toStdString();
    jsonData["name"] = record.value("name").toString().toStdString();
    jsonData["content"] = record.value("content").toString().toStdString();

    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *******************************************
// This handler will be update classbook localization record
// *******************************************

CmdClassbookLocalizationUpdateRecordHandler::CmdClassbookLocalizationUpdateRecordHandler()
    : CmdHandlerBase("classbook_localization_update_record", "Update table with localization by classbookid") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbook_localizationid", "Localization id");
    requireStringParam("name", "Article name");
    requireStringParam("content", "The content of the article");
}

// ---------------------------------------------------------------------

void CmdClassbookLocalizationUpdateRecordHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    int nClassbookLocalizationID = -1;
    if (jsonRequest.find("classbook_localizationid") != jsonRequest.end()) {
        nClassbookLocalizationID = jsonRequest.at("classbook_localizationid").get<int>();
    }

    std::string sName = "";
    if (jsonRequest.find("name") != jsonRequest.end()) {
        sName = jsonRequest["name"];
    }

    std::string sContent = "";
    if (jsonRequest.find("content") != jsonRequest.end()) {
        sContent = jsonRequest["content"];
    }

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    nlohmann::json jsonData;

    QSqlQuery query(db);
    query.prepare("SELECT id FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", nClassbookLocalizationID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This localization doesn't exist"));
        return;
    }

    //Set md5_content hash
    std::string sContentMd5_ = WsjcppHashes::md5_calc_hex(sContent);
    QString md5_content = QString::fromStdString(sContentMd5_);

    query.prepare("UPDATE classbook_localization SET name = :name, content = :content, md5_content = :md5_content, updated = NOW() "
                  "WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", nClassbookLocalizationID);
    query.bindValue(":name", QString::fromStdString(sName));
    query.bindValue(":content", QString::fromStdString(sContent));
    query.bindValue(":md5_content", md5_content);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    query.prepare("SELECT classbookid, lang FROM classbook_localization WHERE id=:id");
    query.bindValue(":id", nClassbookLocalizationID);
    query.exec();
    query.next(); // TODO if no next
    QSqlRecord record = query.record();
    jsonData["classbookid"] = record.value("classbookid").toInt();
    jsonData["classbook_localizationid"] = nClassbookLocalizationID;
    jsonData["lang"] = record.value("lang").toString().toStdString();
    jsonData["name"] = sName;
    jsonData["content"] = sContent;
    jsonData["md5_content"] = md5_content.toStdString();

    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be add classbook proposal record
 * */

CmdClassbookProposalAddRecordHandler::CmdClassbookProposalAddRecordHandler()
    : CmdHandlerBase("classbook_proposal_add_record", "Propose an update of article") {

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbookid", "Classbookid for an article");
    requireStringParam("lang", "Language"); // TODO validator lang
    requireStringParam("name", "Article name");
    requireStringParam("content", "The content of the article");
}

// ---------------------------------------------------------------------

void CmdClassbookProposalAddRecordHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    int nClassbookID = -1;
    if (jsonRequest.find("classbookid") != jsonRequest.end()) {
        nClassbookID = jsonRequest.at("classbookid").get<int>();
    }
    std::string sLang = "";
    if (jsonRequest.find("lang") != jsonRequest.end()) {
        sLang = jsonRequest["lang"];
    }

    std::string sName = "";
    if (jsonRequest.find("name") != jsonRequest.end()) {
        sName = jsonRequest["name"];
    }

    std::string sContent = "";
    if (jsonRequest.find("content") != jsonRequest.end()) {
        sContent = jsonRequest["content"];
    }


    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    nlohmann::json  jsonData;

    QSqlQuery query(db);

    //obtain a current version of classbook name and content
    if (sLang == "en") {
        query.prepare("SELECT name, content FROM classbook WHERE id = :classbookid");
        query.bindValue(":classbookid", nClassbookID);
    } else {
        query.prepare("SELECT name, content FROM classbook_localization WHERE lang = :lang");
        query.bindValue(":lang", QString::fromStdString(sLang));
    }

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This article or localization doesn't exist"));
        return;
    }
    QSqlRecord record = query.record();
    QString name_before = record.value("name").toString();
    QString content_before = record.value("content").toString();

    //Set md5_content hash
    std::string sContentMd5_ = WsjcppHashes::md5_calc_hex(sContent);
    QString md5_content = QString::fromStdString(sContentMd5_);

    //generate uuid
    std::string sUuid = WsjcppCore::createUuid();

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
    query.bindValue(":classbookid", nClassbookID);
    query.bindValue(":uuid", QString::fromStdString(sUuid));
    query.bindValue(":lang", QString::fromStdString(sLang));
    query.bindValue(":name", QString::fromStdString(sName));
    query.bindValue(":content", QString::fromStdString(sContent));
    query.bindValue(":name_before", name_before);
    query.bindValue(":content_before", content_before);
    query.bindValue(":md5_content", md5_content);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    int rowid = query.lastInsertId().toInt();
    jsonData["classbookid"] = nClassbookID;
    jsonData["classbook_proposal_id"] = rowid;
    jsonData["lang"] = sLang;
    jsonData["name"] = sName;
    jsonData["content"] = sContent;
    jsonData["name_before"] = name_before.toStdString();
    jsonData["content_before"] = content_before.toStdString();
    jsonData["md5_content"] = md5_content.toStdString();

    jsonResponse["data"] = jsonData;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be delete classbook proposal record
 * */

CmdClassbookProposalDeleteRecordHandler::CmdClassbookProposalDeleteRecordHandler()
    : CmdHandlerBase("classbook_proposal_delete_record", "Delete a proposal of updating an article") {

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    requireIntegerParam("classbook_proposal_id", "Proposal id");
}

// ---------------------------------------------------------------------

void CmdClassbookProposalDeleteRecordHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    
    int nClassbookProposalID = -1;
    if (jsonRequest.find("classbook_proposal_id") != jsonRequest.end()) {
        nClassbookProposalID = jsonRequest.at("classbook_proposal_id").get<int>();
    }
    // TODO admin password or user password how create this proposal

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT id FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", nClassbookProposalID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This proposal doesn't exist"));
        return;
    }

    query.prepare("DELETE FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", nClassbookProposalID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be info classbook proposal record
 * */

CmdClassbookProposalInfoHandler::CmdClassbookProposalInfoHandler()
    : CmdHandlerBase("classbook_proposal_info", "Find and display all proposal data by id") {

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbook_proposal_id", "Proposal id");
}

// ---------------------------------------------------------------------

void CmdClassbookProposalInfoHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    int nClassbookProposalID = -1;
    if (jsonRequest.find("classbook_proposal_id") != jsonRequest.end()) {
        nClassbookProposalID = jsonRequest.at("classbook_proposal_id").get<int>();
    }

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    nlohmann::json jsonData;

    QSqlQuery query(db);
    query.prepare("SELECT id FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", nClassbookProposalID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This proposal doesn't exist"));
        return;
    }

    query.prepare("SELECT classbookid, lang, name, content FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", nClassbookProposalID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This proposal doesn't exist"));
        return;
    }

    QSqlRecord record = query.record();
    jsonData["classbookid"] = record.value("classbookid").toInt();
    jsonData["id"] = nClassbookProposalID;
    jsonData["lang"] = record.value("lang").toString().toStdString();
    jsonData["name"] = record.value("name").toString().toStdString();
    jsonData["content"] = record.value("content").toString().toStdString();

    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be list classbook proposal record
 * */

CmdClassbookProposalListHandler::CmdClassbookProposalListHandler()
    : CmdHandlerBase("classbook_proposal_list", "Display list of proposals by classbookid") {

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbookid", "Classbookid for an article");
    optionalStringParam("lang", "Language"); // TODO validator lang
}

// ---------------------------------------------------------------------

void CmdClassbookProposalListHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);

    nlohmann::json jsonData = nlohmann::json::array();

    QString sQuery;
    QMap<QString, QJsonValue> mapFilter;

    //checkout and validation of classbookid
    int nClassbookID = 0;
    if (jsonRequest.find("classbookid") != jsonRequest.end()) {
        nClassbookID = jsonRequest.at("classbookid").get<int>();
        query.prepare("SELECT id FROM classbook WHERE id = :classbookid");
        query.bindValue(":classbookid", nClassbookID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (!query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "This article doesn't exist"));
            return;
        }
        mapFilter.insert("classbookid", nClassbookID);
    }

    //checkout of lang and generation of query's bone
    bool bLangConatins = false;
    std::string sLang = "";
    if (jsonRequest.find("lang") != jsonRequest.end()) {
        bLangConatins = true;
        sLang = jsonRequest["lang"];
        mapFilter.insert("lang", QString::fromStdString(sLang).trimmed());
        sQuery = "SELECT id, name FROM classbook_proposal";
    }
    else sQuery = "SELECT id, name, lang FROM classbook_proposal";

    //generation of the rest of the query
    if (mapFilter.size() > 0) {
        sQuery += " WHERE ";
    }
    bool bFirst = true;
    foreach (QString key, mapFilter.keys()) {
        if (!bFirst) {
            sQuery += " AND ";
        }
        bFirst = false;
        sQuery +=  key + " = :" + key;
    }
    query.prepare(sQuery);

    //binding of values
    foreach (QString key, mapFilter.keys()) {
        QMap<QString, QJsonValue>::const_iterator v = mapFilter.lowerBound(key);
        if (key=="classbookid") {
            query.bindValue(":" + key, v.value().toInt());
        } else {
            query.bindValue(":" + key, v.value());
        }
    }
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        nlohmann::json jsonItem;
        jsonItem["id"] = record.value("id").toInt();
        jsonItem["classbookid"] = nClassbookID;
        if (bLangConatins) {
            jsonItem["lang"] = sLang;
        } else {
            jsonItem["lang"] = record.value("lang").toString().trimmed().toStdString();
        }
        jsonItem["name"] = record.value("name").toString().toStdString();
        jsonData.push_back(jsonItem);
    }

    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be prepare classbook proposal record
 * */
CmdClassbookProposalPrepareMergeRecordHandler::CmdClassbookProposalPrepareMergeRecordHandler()
    : CmdHandlerBase("classbook_propasal_prepare_merge_record", "Prepare to merge updating requests") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbook_proposal_id", "Proposal id");
}

// ---------------------------------------------------------------------

void CmdClassbookProposalPrepareMergeRecordHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();

    int nClassbookProposalID = -1;
    if (jsonRequest.find("classbook_proposal_id") != jsonRequest.end()) {
        nClassbookProposalID = jsonRequest.at("classbook_proposal_id").get<int>();
    }

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    nlohmann::json jsonData;

    QSqlQuery query(db);
    QSqlRecord record = query.record();
    query.prepare("SELECT id FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", nClassbookProposalID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This proposal doesn't exist"));
        return;
    }

    query.prepare("SELECT content FROM classbook WHERE id IN (SELECT classbookid FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", nClassbookProposalID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    std::string curtxt = record.value("content").toString().toStdString();

    query.prepare("SELECT content, content_before FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", nClassbookProposalID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    std::string txt1 = record.value("content").toString().toStdString();
    std::string txt2 = record.value("content_before").toString().toStdString();
    std::vector<WsjcppDiffTextRow *> arr1, arr2;
    WsjcppDiffText::merge(curtxt, txt1, txt2, arr1, arr2);

    // TODO final merge, lang checkout, update output (with data)

    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be prepare classbook proposal record
 * */
CmdClassbookProposalApproveHandler::CmdClassbookProposalApproveHandler()
    : CmdHandlerBase("classbook_propasal_approve", "Approve updating requests") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbook_proposal_id", "Proposal id");
}

// ---------------------------------------------------------------------

void CmdClassbookProposalApproveHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    int nClassbookProposalID = -1;
    if (jsonRequest.find("classbook_proposal_id") != jsonRequest.end()) {
        nClassbookProposalID = jsonRequest.at("classbook_proposal_id").get<int>();
    }

    if (nClassbookProposalID == -1) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This proposal doesn't exist"));
        return;
    }

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    query.prepare("SELECT classbookid, content FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", nClassbookProposalID);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This proposal doesn't exist"));
        return;
    }

    QSqlRecord propRecord = query.record();
    int nClassbookID = propRecord.value("classbookid").toInt();
    std::string sContent = propRecord.value("content").toString().toStdString();
    std::string sContentMd5_ = WsjcppHashes::md5_calc_hex(sContent);

    // TODO: add lang support

    query.prepare("UPDATE classbook SET content = :content, md5_content = :md5_content WHERE id = :classbookid");
    query.bindValue(":classbookid", nClassbookID);
    query.bindValue(":content", QString::fromStdString(sContent));
    query.bindValue(":md5_content", QString::fromStdString(sContentMd5_));
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonResponse;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be prepare classbook proposal record
 * */
CmdClassbookProposalUpdateHandler::CmdClassbookProposalUpdateHandler()
    : CmdHandlerBase("classbook_propasal_update", "Approve updating requests") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("classbook_proposal_id", "Proposal id");
    requireStringParam("content", "new content");
}

// ---------------------------------------------------------------------

void CmdClassbookProposalUpdateHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    int nClassbookProposalID = -1;
    if (jsonRequest.find("classbook_proposal_id") != jsonRequest.end()) {
        nClassbookProposalID = jsonRequest.at("classbook_proposal_id").get<int>();
    }

    if (nClassbookProposalID == -1) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "This proposal doesn't exist"));
        return;
    }

    std::string sContent;
    if (jsonRequest.find("content") != jsonRequest.end()) {
        sContent = jsonRequest.at("content").get<std::string>();
    }
    std::string sContentMd5_ = WsjcppHashes::md5_calc_hex(sContent);

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    query.prepare("UPDATE classbook_proposal SET content = :content, md5_content = :md5_content WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", nClassbookProposalID);
    query.bindValue(":content", QString::fromStdString(sContent));
    query.bindValue(":md5_content", QString::fromStdString(sContentMd5_));
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonResponse;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*!
 * This handler will be return classbook content (duplicate handler ? )
 * */

CmdClassbookHandler::CmdClassbookHandler()
    : CmdHandlerBase("classbook", "Return classbook contents") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

}

// ---------------------------------------------------------------------

void CmdClassbookHandler::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    nlohmann::json jsonContents = nlohmann::json::array();

    QSqlQuery query(db);
    query.prepare("SELECT * FROM classbook ORDER BY id ASC");
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        QString uuid = record.value("uuid").toString();
        nlohmann::json jsonItem;
        jsonItem["id"] = record.value("id").toInt();
        jsonItem["parentid"] = record.value("parentid").toInt();
        jsonItem["uuid"] = uuid.toStdString();
        jsonItem["parentuuid"] = record.value("parentuuid").toString().toStdString();
        jsonItem["name_ru"] = record.value("name_ru").toString().toStdString();
        jsonItem["name_en"] = record.value("name_en").toString().toStdString();
        jsonItem["name"] = record.value("name_en").toString().toStdString();
        jsonItem["link"] = QString("files/classbook/" + uuid + "_en.md").toStdString();
        jsonContents.push_back(jsonItem);
    }

    jsonResponse["items"] = jsonContents;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
