#include <employ_scoreboard.h>
#include <employ_database.h>
#include <QSqlQuery>
#include <QSqlRecord>

REGISTRY_EMPLOY(EmployScoreboard)

// ---------------------------------------------------------------------

EmployScoreboard::EmployScoreboard()
    : EmployBase(EmployScoreboard::name(), {EmployDatabase::name()}) {

}

// ---------------------------------------------------------------------

bool EmployScoreboard::init(){

    return true;
}

// ---------------------------------------------------------------------

void EmployScoreboard::loadSync(){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());
    m_vRows.clear(); // TODO fix memory leaks

    QSqlQuery query(db);
    query.prepare("SELECT id,nick,logo,university,rating FROM users WHERE role = 'user' ORDER BY rating DESC");
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        User *pUser = new User();
        pUser->userid = record.value("id").toInt();
        pUser->nick = record.value("nick").toString().toHtmlEscaped();
        pUser->university = record.value("university").toString().toHtmlEscaped();
        pUser->logo = record.value("logo").toString().toHtmlEscaped();
        int rating = record.value("rating").toInt();
        int nRow = findScoreboardRowByRating(rating);
        if(nRow < 0){
            ScoreboardRow *pRow = new ScoreboardRow();
            pRow->rating = rating;
            pRow->vUsers.push_back(pUser);
            m_vRows.push_back(pRow);
        }else{
            m_vRows[nRow]->vUsers.push_back(pUser);
        }
    }

    for(int i = 0; i < m_vRows.size(); i++){
        m_vRows[i]->place = i+1;
    }
}

// ---------------------------------------------------------------------

int EmployScoreboard::findScoreboardRowByRating(int rating){
    for(int i = 0; i < m_vRows.size(); i++){
        if(m_vRows[i]->rating == rating){
            return i;
        }
    }
    return -1;
}

// ---------------------------------------------------------------------

QJsonArray EmployScoreboard::toJsonArray(){
    QJsonArray arr;
    for(int i = 0; i < m_vRows.size(); i++){
        QJsonObject row;
        row["rating"] = m_vRows[i]->rating;
        row["place"] = m_vRows[i]->place;
        QJsonArray users;
        for(int u = 0; u < m_vRows[i]->vUsers.size(); u++){
            QJsonObject user;
            user["userid"] = m_vRows[i]->vUsers[u]->userid;
            user["nick"] = m_vRows[i]->vUsers[u]->nick;
            user["logo"] = m_vRows[i]->vUsers[u]->logo;
            user["university"] = m_vRows[i]->vUsers[u]->university;
            user["rating"] = m_vRows[i]->rating;
            users.push_back(user);
        }
        row["users"] = users;
        arr.push_back(row);
    }
    return arr;
}

// ---------------------------------------------------------------------

int EmployScoreboard::count(){
    return m_vRows.size();
}
