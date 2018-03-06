#include <cmd_classbook_export_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <QTemporaryDir>
#include <QFile>
#include <quazip.h>
#include <quazipfile.h>
#include <quazipfileinfo.h>

CmdClassbookExportHandler::CmdClassbookExportHandler(){
    m_vInputs.push_back(CmdInputDef("output").required().string_().description("The output file format"));
    m_vInputs.push_back(CmdInputDef("lang").required().string_().description("The output file format"));
    m_vInputs.push_back(CmdInputDef("zip").optional().bool_().description("Zipping the output"));
}

std::string CmdClassbookExportHandler::cmd(){
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

const std::vector<CmdInputDef> &CmdClassbookExportHandler::inputs(){
    return m_vInputs;
}

QString CmdClassbookExportHandler::description(){
    return "Export classbook's articles to html or markdown, optionally in zip archive.";
}

QStringList CmdClassbookExportHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookExportHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());
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
