#ifndef EMPLOY_GAMES_H
#define EMPLOY_GAMES_H

#include <wsjcpp_employees.h>
#include <model_game.h>

class EmployGames : public WSJCppEmployBase {
    public:

        EmployGames();
        static std::string name() { return "EmployGames"; }
        virtual bool init();

        bool findGame(int nLocalId, ModelGame &modelGame);
        bool findGame(const std::string &sUuid, ModelGame &modelGame);
        EmployResult addGame(const ModelGame &modelGame, std::string &sError);
        EmployResult updateGame(const ModelGame &modelGame, std::string &sError);
        EmployResult removeGame(const std::string &sUuid);

    private:
        std::string TAG;
        std::vector<ModelGame*> m_vectCacheGame;
        std::map<std::string, ModelGame*> m_mapCacheGames;
};

#endif // EMPLOY_GAMES_H
