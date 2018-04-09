#ifndef EMPLOY_SCOREBOARD_H
#define EMPLOY_SCOREBOARD_H

#include <employees.h>
#include <QVector>
#include <QJsonArray>

class EmployScoreboard : public EmployBase {
    public:
       EmployScoreboard();
       static std::string name() { return "EmployScoreboard"; }
       virtual bool init();

       void loadSync();
       int count();
       QJsonArray toJsonArray();

   private:
       struct User{
           int userid = 0;
           QString logo = "";
           QString nick = "";
           QString university = "";
       };

       struct ScoreboardRow {
           int place = 0;
           int rating = 0;
           QVector<User *> vUsers;
       };
       QVector<ScoreboardRow *> m_vRows;
       int findScoreboardRowByRating(int rating);
};

#endif // EMPLOY_SCOREBOARD_H
