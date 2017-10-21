#include <cmd_game_update_logo_handler.h>
#include <runtasks.h>
#include <memory_cache_serversettings.h>

#include <QJsonArray>
#include <QCryptographicHash>
#include <QFile>
#include <QImage>

CmdGameUpdateLogoHandler::CmdGameUpdateLogoHandler(){
    m_vInputs.push_back(CmdInputDef("gameid").integer_().required().description("GameID"));
    m_vInputs.push_back(CmdInputDef("image_png_base64").string_().required().description("Image PNG in Base64"));
}

QString CmdGameUpdateLogoHandler::cmd(){
    return "game_update_logo";
}

bool CmdGameUpdateLogoHandler::accessUnauthorized(){
	return false;
}

bool CmdGameUpdateLogoHandler::accessUser(){
	return false;
}

bool CmdGameUpdateLogoHandler::accessTester(){
	return false;
}

bool CmdGameUpdateLogoHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdGameUpdateLogoHandler::inputs(){
	return m_vInputs;
};

QString CmdGameUpdateLogoHandler::description(){
    return "Update game logo";
}

QStringList CmdGameUpdateLogoHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameUpdateLogoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    int nGameID = obj["gameid"].toInt();

    IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serversettings");
    if(pMemoryCache == NULL){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
        return;
    }

    MemoryCacheServerSettings *pMemoryCacheServerSettings = dynamic_cast<MemoryCacheServerSettings*>(pMemoryCache);
    QString sBasePath = pMemoryCacheServerSettings->getSettString("server_folder_games");

    QString sFilename = sBasePath + QString::number(nGameID) + ".png";

    QByteArray baImagePNGBase64;

    QString sImagePngBase64 = obj["image_png_base64"].toString();
    baImagePNGBase64.append(sImagePngBase64);
    QByteArray baImagePNG = QByteArray::fromBase64(baImagePNGBase64); // .fromBase64(baImagePNGBase64);

    if(baImagePNG.size() == 0){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(400, "Could not decode base64"));
        return;
    }

    QImage img = QImage::fromData(baImagePNG,"PNG");
    if(img.height() == 0 && img.width() == 0){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(400, "Could not decode bytearray to png"));
        return;
    }

    // TODO resize icon of game

    QFile file(sFilename);

    if(file.exists()){
        if(!file.remove()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "Could not remove old file"));
            return;
        }
    }
    if(!img.save(sFilename)){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "Could not save new file"));
        return;
    }

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonData);
}
