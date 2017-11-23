#include <cmd_classbook_export_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QFile>

CmdClassbookExportHandler::CmdClassbookExportHandler(){
    m_vInputs.push_back(CmdInputDef("output").required().string_().description("The output file format"));
    m_vInputs.push_back(CmdInputDef("lang").required().string_().description("The output file format"));
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
};

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

    QString lang = obj.value("lang").toString().trimmed();
    QMap <int, QString> name_of_articles;
    if (lang == "en"){
        query.prepare("SELECT id, name FROM classbook");
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

    QFile file;
    file.setFileName("/tmp/export.html");
    file.open(QIODevice::ReadWrite);
    QTextStream out(&file);
    out << "<!DOCTYPE HTML><html><head><title>Freehackquest classbook</title>";
    out << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><meta charset=\"utf-8\"></head>";
    out << "<body><h1> Freehackquest Classbook</h1>";
    out << "<h2>Table of contents</h2>";
    QMap <int, QString>::iterator i;
    for (i = name_of_articles.begin(); i != name_of_articles.end(); ++i){
        out << "<h3><a href=#" << i.key() << ">" << i.value().toUtf8() << "</a></h3>";
    }

    if (lang == "en"){
        query.prepare("SELECT id, name, content FROM classbook");
        query.exec();
        while (query.next()){
            QSqlRecord record = query.record();
            out << "<article id=" << record.value("id").toInt() << "><h2>" << record.value("name").toString().toUtf8() << "</h2>";
            out << "<p>" << record.value("content").toString().toUtf8() << "</p></article>";
        }
    } else {
        query.prepare("SELECT classbookid, name, content FROM classbook_localization WHERE lang=:lang");
        query.bindValue(":lang", lang);
        query.exec();
        while (query.next()){
            QSqlRecord record = query.record();
            out << "<article><h2 id =" << record.value("classbookid").toInt() << ">" << record.value("name").toString().toUtf8() << "</h2>";
            out << "<p>" << record.value("content").toString().toUtf8() << "</p></article>";
        }
    }
    out << "</body></html>";

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(cmd());
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["data"] = QString::fromUtf8(file.readAll());
    jsonResponse["result"] = QJsonValue("DONE");
    file.remove();
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}
