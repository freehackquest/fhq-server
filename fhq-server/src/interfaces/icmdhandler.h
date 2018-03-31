#ifndef INTERFACES_ICMDHANDLER_H
#define INTERFACES_ICMDHANDLER_H

#include <QString>
#include <QJsonObject>
#include <QVector>
#include <model_request.h>
#include <model_command_access.h>
#include <cmd_input_def.h>

class ICmdHandler {
	public:
        virtual std::string cmd() = 0;
        virtual std::string description() = 0;
        virtual const ModelCommandAccess &access() = 0;
        virtual const std::vector<CmdInputDef> &inputs() = 0;
        virtual void handle(ModelRequest *pRequest) = 0;
};

#endif // INTERFACES_ICMDHANDLER_H
