#ifndef CREATE_CMD_HANDLERS_H
#define CREATE_CMD_HANDLERS_H

#include "../interfaces/icmdhandler.h"
#include <QString>
#include <QMap>

void create_cmd_handlers(QMap<QString, ICmdHandler *> &pHandlers);

#endif // CREATE_CMD_HANDLERS_H
