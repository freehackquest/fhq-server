#ifndef EMPLOY_SCOREBOARD_H
#define EMPLOY_SCOREBOARD_H

#include <employees.h>
#include <json.hpp>

class EmployScoreboard : public WsjcppEmployBase {
    public:
        EmployScoreboard();
        static std::string name() { return "EmployScoreboard"; }
        virtual bool init();
        virtual bool deinit() override;
        void loadSync();
        int count();
        nlohmann::json toJson();
       
        void asyncUpdatedQuestScore(int nQuestID);
        void asyncUpdatedUserRating(int nUserID);
        void asyncUpdatedLeaksScore(int nUserID);

   private:
       struct User{
           int userid = 0;
           std::string logo = "";
           std::string nick = "";
           std::string university = "";
       };

       struct ScoreboardRow {
           int place = 0;
           int rating = 0;
           std::vector<User *> vUsers;
       };
       std::vector<ScoreboardRow *> m_vRows;
       int findScoreboardRowByRating(int rating);
};

#endif // EMPLOY_SCOREBOARD_H
