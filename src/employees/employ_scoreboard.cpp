#include <employ_scoreboard.h>
#include <employ_database.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMap>
#include <QString>
#include <QVariant>

REGISTRY_WJSCPP_EMPLOY(EmployScoreboard)

// ---------------------------------------------------------------------

EmployScoreboard::EmployScoreboard()
    : WsjcppEmployBase(EmployScoreboard::name(), {EmployDatabase::name()}) {
    TAG = "EmployScoreboard";
}

// ---------------------------------------------------------------------

bool EmployScoreboard::init() {
    return true;
}

// ---------------------------------------------------------------------

bool EmployScoreboard::deinit() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------

void EmployScoreboard::loadSync() {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());
    m_vRows.clear(); // TODO fix memory leaks

    QSqlQuery query(db);
    query.prepare("SELECT id,nick,logo,university,rating FROM users WHERE role = 'user' ORDER BY rating DESC");
    if (!query.exec()) {
        WsjcppLog::throw_err(TAG, "EmployDatabase::loadAllSettings => " + query.lastError().text().toStdString());
        return;
    };
    while (query.next()) {
        QSqlRecord record = query.record();
        User *pUser = new User();
        pUser->userid = record.value("id").toInt();
        pUser->nick = record.value("nick").toString().toHtmlEscaped().toStdString();
        pUser->university = record.value("university").toString().toHtmlEscaped().toStdString();
        pUser->logo = "public/" + record.value("logo").toString().toHtmlEscaped().toStdString();
        int rating = record.value("rating").toInt();
        int nRow = findScoreboardRowByRating(rating);
        if (nRow < 0) {
            ScoreboardRow *pRow = new ScoreboardRow();
            pRow->rating = rating;
            pRow->vUsers.push_back(pUser);
            m_vRows.push_back(pRow);
        } else {
            m_vRows[nRow]->vUsers.push_back(pUser);
        }
    }

    for (unsigned int i = 0; i < m_vRows.size(); i++) {
        m_vRows[i]->place = i+1;
    }
}

// ---------------------------------------------------------------------

int EmployScoreboard::findScoreboardRowByRating(int rating) {
    for (unsigned int i = 0; i < m_vRows.size(); i++) {
        if (m_vRows[i]->rating == rating) {
            return i;
        }
    }
    return -1;
}

// ---------------------------------------------------------------------

nlohmann::json EmployScoreboard::toJson() {
    nlohmann::json jsonScoreboard = nlohmann::json::array();
    
    for (unsigned int i = 0; i < m_vRows.size(); i++) {
        nlohmann::json jsonRow;
        jsonRow["rating"] = m_vRows[i]->rating;
        jsonRow["place"] = m_vRows[i]->place;
        
        nlohmann::json jsonUsers = nlohmann::json::array();;
        for (unsigned int u = 0; u < m_vRows[i]->vUsers.size(); u++) {
            nlohmann::json jsonUser;
            jsonUser["userid"] = m_vRows[i]->vUsers[u]->userid;
            jsonUser["nick"] = m_vRows[i]->vUsers[u]->nick;
            jsonUser["logo"] = m_vRows[i]->vUsers[u]->logo;
            jsonUser["university"] = m_vRows[i]->vUsers[u]->university;
            jsonUser["rating"] = m_vRows[i]->rating;
            jsonUsers.push_back(jsonUser);
        }
        jsonRow["users"] = jsonUsers;
        jsonScoreboard.push_back(jsonRow);
    }
    return jsonScoreboard;
}

// ---------------------------------------------------------------------

int EmployScoreboard::count() {
    return m_vRows.size();
}

// ---------------------------------------------------------------------

void EmployScoreboard::asyncUpdatedQuestScore(int /*nQuestID*/) {
    // TODO
    // When quest score changed
}

// ---------------------------------------------------------------------

void EmployScoreboard::asyncUpdatedUserRating(int /*nUserID*/) {
    // TODO
    // When user score changed
}
       
// ---------------------------------------------------------------------

void EmployScoreboard::asyncUpdatedLeaksScore(int /*nLeakID*/) {
    // TODO
    // When leak score changed
}
       
