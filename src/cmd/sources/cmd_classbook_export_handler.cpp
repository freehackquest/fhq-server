#include <cmd_classbook_export_handler.h>
#include <QJsonArray>
#include <QSqlError>
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
    if (lang == "en"){
        query.prepare("SELECT name, content FROM classbook");
        query.exec();
    } else {
        query.prepare("SELECT name, content FROM classbook_localization WHERE lang=:lang");
        query.bindValue(":lang", lang);
        query.exec();
    }

    QFile file;
    file.setFileName("/tmp/export.html");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << "<!DOCTYPE HTML><html><head><title>Freehackquest classbook</title>";
    out << "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css\"";
    out << "integrity=\"sha384-PsH8R72JQ3SOdhVi3uxftmaW6Vc51MKb0q5P2rRUpPvrszuE4W1povHYgTpBfshb\" crossorigin=\"anonymous\">";
    out << "</head><main role=\"main\" class=\"container\"><div class=\"col-sm-10 blog-main\">";
    while (query.next()){
        QSqlRecord record = query.record();
        out << "<div class=\"blog-post\">";
        out << "<h2 class=\"blog-post-title\">" << record.value("name").toString().toUtf8() << "</h2>";
        out << "<p>" << record.value("content").toString().toUtf8() << "</p></div>";
    }
    out << "</div></main></html>";
    file.close();
    file.open(QIODevice::ReadOnly);

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(cmd());
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["data"] = QString::fromUtf8(file.readAll());
    jsonResponse["result"] = QJsonValue("DONE");
    file.close();
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}

