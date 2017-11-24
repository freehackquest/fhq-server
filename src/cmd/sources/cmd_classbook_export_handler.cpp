#include <cmd_classbook_export_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QFile>
#include <quazip.h>
#include <quazipfile.h>
#include <quazipfileinfo.h>

CmdClassbookExportHandler::CmdClassbookExportHandler(){
    m_vInputs.push_back(CmdInputDef("output").required().string_().description("The output file format"));
    m_vInputs.push_back(CmdInputDef("lang").required().string_().description("The output file format"));
    m_vInputs.push_back(CmdInputDef("zip").optional().bool_().description("The output file format"));
}

QString CmdClassbookExportHandler::cmd(){
    return "classbook_export";
}

bool CmdClassbookExportHandler::accessUnauthorized(){
    return true;
}

bool CmdClassbookExportHandler::accessUser(){
    return true;
}

bool CmdClassbookExportHandler::accessTester(){
    return true;
}

bool CmdClassbookExportHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookExportHandler::inputs(){
    return m_vInputs;
}

QString CmdClassbookExportHandler::description(){
    return "Export classbook's articles to html or zip archive";
}

QStringList CmdClassbookExportHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookExportHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());
    QSqlQuery query(db);

    QFile file;
    file.setFileName("/tmp/freehackquest_export_F3h233h2");
    file.open(QIODevice::WriteOnly);
    QString lang = obj.value("lang").toString().trimmed();
    if (obj.value("output") == "html"){
        createHtml(&file, lang, query);
    }
    if (obj.value("output") == "markdown"){
        createMD(&file, lang, query);
    }
    file.close();
    file.open(QIODevice::ReadOnly);

    QJsonObject jsonResponse;
    if (obj.contains("zip") and obj.value("zip").toBool()){
        QString tmpDir = QDir::tempPath();
        QString tmpZipFile = tmpDir + "/freehackquest-classbook_" + lang + ".zip";
        // prepare zip archive
        QuaZip zip(tmpZipFile);
        zip.open(QuaZip::mdCreate);
        QuaZipFile export_zipfile(&zip);

        QString name;
        if (obj.value("output") == "html"){
            name = "freehackquest-classbook.html";}
        if (obj.value("output") == "markdown"){
            name = "freehackquest-classbook.md";
        }
        export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(name));
        // After .toString(), you should specify a text codec to use to encode the
        // string data into the (binary) file. Here, I use UTF-8:
        export_zipfile.write(file.readAll());
        export_zipfile.close();
        zip.close();
        QFile fileZip(tmpZipFile);
        if (!fileZip.open(QIODevice::ReadOnly)){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, "Could not open zip file"));
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

    jsonResponse["cmd"] = QJsonValue(cmd());
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["result"] = QJsonValue("DONE");
    pWebSocketServer->sendMessage(pClient, jsonResponse);
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
