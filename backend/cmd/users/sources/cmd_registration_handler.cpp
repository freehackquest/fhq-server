#include <cmd_registration_handler.h>
#include <runtasks.h>
#include <log.h>
#include <model_usertoken.h>
#include <QCryptographicHash>
#include <QUuid>
#include <SmtpMime>

CmdRegistrationHandler::CmdRegistrationHandler(){
    TAG = "CmdRegistrationHandler";

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(false);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("email").required().email_().description("E-mail"));
    m_vInputs.push_back(CmdInputDef("university").required().string_().description("University"));
}

// ---------------------------------------------------------------------

std::string CmdRegistrationHandler::cmd(){
    return "registration";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdRegistrationHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdRegistrationHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdRegistrationHandler::description(){
    return "Method for registration";
}

// ---------------------------------------------------------------------

void CmdRegistrationHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
	
    //EmploySettings *pSettings = findEmploy<EmploySettings>();

    QString sEmail = jsonRequest["email"].toString();

    QString sUniversity = jsonRequest["university"].toString();

    QSqlDatabase db = *(pRequest->server()->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :email");
    query.bindValue(":email", sEmail);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if (query.next()) {
        Log::err(TAG, "User already exists " + sEmail);
        pRequest->sendMessageError(cmd(), Error(403, "This email already exists"));
        return;
    }

    // // generate random password
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 12; // assuming you want random strings of 12 characters
    QString sPassword;
    for(int i=0; i<randomStringLength; ++i){
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        sPassword.append(nextChar);
    }

    QString sPassword_sha1 = sEmail.toUpper() + sPassword;
    sPassword_sha1 = QString("%1").arg(QString(QCryptographicHash::hash(sPassword_sha1.toUtf8(),QCryptographicHash::Sha1).toHex()));

    // generate random nick
    const QString possibleCharacters2("ABCDEFH0123456789");
    const int randomStringLength2 = 8; // assuming you want random strings of 12 characters
    QString sNick;
    for(int i=0; i<randomStringLength2; ++i){
        int index = qrand() % possibleCharacters2.length();
        QChar nextChar = possibleCharacters2.at(index);
        sNick.append(nextChar);
    }
    sNick = "hacker-" + sNick;

    QSqlQuery query_insert(db);
    query_insert.prepare(""
                         "INSERT INTO users ("
                         "   uuid, "
                         "   email, "
                         "   pass, "
                         "   role, "
                         "   nick,"
                         "   logo,"
                         "   dt_create,"
                         "   dt_last_login,"
                         "   last_ip,"
                         "   status,"
                         "   country,"
                         "   region,"
                         "   city,"
                         "   university,"
                         "   latitude,"
                         "   longitude,"
                         "   rating,"
                         "   about)"
                         "VALUES("
                         "   :uuid, "
                         "   :email, "
                         "   :pass, "
                         "   :role, "
                         "   :nick,"
                         "   :logo,"
                         "   NOW(),"
                         "   NOW(),"
                         "   :last_ip,"
                         "   :status,"
                         "   :country,"
                         "   :region,"
                         "   :city,"
                         "   :university,"
                         "   :latitude,"
                         "   :longitude,"
                         "   :rating,"
                         "   :about);"
    );

    QString sLastIP = pRequest->client()->peerAddress().toString();

    QString sUuid = QUuid::createUuid().toString();
    sUuid = sUuid.mid(1,sUuid.length()-2);
    sUuid = sUuid.toUpper();

    query_insert.bindValue(":uuid", sUuid);
    query_insert.bindValue(":email", sEmail);
    query_insert.bindValue(":pass", sPassword_sha1);
    query_insert.bindValue(":role", "user");
    query_insert.bindValue(":nick", sNick);
    query_insert.bindValue(":logo", "files/users/0.png");
    query_insert.bindValue(":last_ip", "");
    query_insert.bindValue(":status", "activated");
    query_insert.bindValue(":country", "");
    query_insert.bindValue(":region", "");
    query_insert.bindValue(":city", "");
    query_insert.bindValue(":university", sUniversity);
    query_insert.bindValue(":latitude", 0);
    query_insert.bindValue(":longitude", 0);
    query_insert.bindValue(":rating", 0);
    query_insert.bindValue(":about", "");

    if(!query_insert.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query_insert.lastError().text()));
        return;
    }

    int nUserID = query_insert.lastInsertId().toInt();

    RunTasks::AddPublicEvents(pRequest->server(), "users", "New user #" + QString::number(nUserID) + "  " + sNick);

    QString sSubject = "Registration on FreeHackQuest";
    QString sContext = "Welcome to FreeHackQuest 2017!\n"
                       "You login: " + sEmail + "\n"
                       "You password: " + sPassword + "\n";

    RunTasks::MailSend(pRequest->server(), sEmail, sSubject, sContext);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
    RunTasks::UpdateUserLocation(pRequest->server(), nUserID, sLastIP);
}
