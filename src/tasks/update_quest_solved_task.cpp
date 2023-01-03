#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSqlQuery>  // TODO redesign
#include <QSqlRecord> // TODO redesign
#include <QThread>
#include <employ_database.h>
#include <update_quest_solved_task.h>

UpdateQuestSolvedTask::UpdateQuestSolvedTask(int nQuestID) {
  m_nQuestID = nQuestID;
  TAG = "UpdateQuestSolvedTask";
}

UpdateQuestSolvedTask::~UpdateQuestSolvedTask() {}

void UpdateQuestSolvedTask::run() {
  WsjcppLog::info(TAG, "questid " + std::to_string(m_nQuestID));
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());

  QSqlQuery query(db);
  query.prepare("UPDATE quest q "
                "SET "
                " count_user_solved = ( "
                "     SELECT COUNT(*) FROM users_quests"
                "     INNER JOIN users ON users.id = users_quests.userid"
                "     WHERE questid = :questid AND users.role = 'user'"
                " ) WHERE q.idquest = :questid2;");

  query.bindValue(":questid", m_nQuestID);
  query.bindValue(":questid2", m_nQuestID);

  if (!query.exec()) {
    WsjcppLog::err(TAG, query.lastError().text().toStdString());
  }
}
