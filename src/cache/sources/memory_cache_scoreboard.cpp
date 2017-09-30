#include <memory_cache_scoreboard.h>

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDateTime>
#include <QDir>

// IMemoryCache
QString MemoryCacheScoreboard::name(){
	return "scoreboard";
}

// ---------------------------------------------------------------------

MemoryCacheScoreboard::MemoryCacheScoreboard(IWebSocketServer *pWebSocketServer){
	m_pWebSocketServer = pWebSocketServer;
}

// ---------------------------------------------------------------------

void MemoryCacheScoreboard::loadSync(){
	QSqlDatabase db = *(m_pWebSocketServer->database());
	m_vRows.clear(); // TODO fix memory leaks

	QSqlQuery query(db);
	query.prepare("SELECT id,nick,logo,rating FROM users WHERE role = 'user' ORDER BY rating DESC");
	query.exec();
	while (query.next()) {
		QSqlRecord record = query.record();
		User *pUser = new User();
		pUser->userid = record.value("id").toInt();
		pUser->nick = record.value("nick").toString(); // TODO htmlspecialchars
		pUser->logo = record.value("logo").toString(); // TODO htmlspecialchars
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

int MemoryCacheScoreboard::findScoreboardRowByRating(int rating){
	for(int i = 0; i < m_vRows.size(); i++){
		if(m_vRows[i]->rating == rating){
			return i;
		}
	}
	return -1;
}

// ---------------------------------------------------------------------

QJsonArray MemoryCacheScoreboard::toJsonArray(){
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
			user["rating"] = m_vRows[i]->rating;
			users.push_back(user);
		}
		row["users"] = users;
		arr.push_back(row);
	}
	return arr;
}

// ---------------------------------------------------------------------

int MemoryCacheScoreboard::count(){
	return m_vRows.size();
}
