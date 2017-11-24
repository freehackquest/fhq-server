#include "create_cmd_users_handlers.h"
#include <log.h>
#include <cmd_game_create_handler.h>
#include <cmd_game_delete_handler.h>
#include <cmd_game_export_handler.h>
#include <cmd_game_import_handler.h>
#include <cmd_game_info_handler.h>
#include <cmd_games_handler.h>
#include <cmd_game_update_handler.h>
#include <cmd_game_update_logo_handler.h>

void create_cmd_games_handlers(QMap<QString, ICmdHandler *> &pHandlers){
    QString TAG = "create_cmd_users_handlers";
    QVector<ICmdHandler *> v;

    v.push_back(new CmdGameCreateHandler());
    v.push_back(new CmdGameDeleteHandler());
    v.push_back(new CmdGameExportHandler());
    v.push_back(new CmdGameImportHandler());
    v.push_back(new CmdGameInfoHandler());
    v.push_back(new CmdGameUpdateHandler());
    v.push_back(new CmdGameUpdateLogoHandler());
    v.push_back(new CmdGamesHandler());

    for(int i = 0; i < v.size(); i++){
            QString cmd = v[i]->cmd();
            if(pHandlers.contains(cmd)){
                    Log::warn(TAG, "cmd_handler for command " + cmd + " - already registered and will be skipped");
            }else{
                    pHandlers[cmd] = v[i];
            }
    }
}
