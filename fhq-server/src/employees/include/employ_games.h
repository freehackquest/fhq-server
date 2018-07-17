#ifndef EMPLOY_GAMES_H
#define EMPLOY_GAMES_H

#include <employees.h>
#include <model_game.h>

class EmployGames : public EmployBase {
    public:

        EmployGames();
        static std::string name() { return "EmployGames"; }
        virtual bool init();

        ModelGame* findGameByLocalId(int nLocalId);
        ModelGame* findGameByUuid(const std::string &sUuid);
        EmployResult addGame(ModelGame* pModelGame, std::string &sError);
        EmployResult removeGame(const std::string &sUuid);

    private:
        std::string TAG;
        std::vector<ModelGame*> m_vectCacheGame;
        std::map<std::string, ModelGame*> m_mapCacheGames;

        bool dir_exists(const char* filePath);
};

#endif // EMPLOY_GAMES_H
