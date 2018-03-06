#ifndef INTERFACES_ICMDHANDLER_H
#define INTERFACES_ICMDHANDLER_H

#include <QString>
#include <QJsonObject>
#include <QVector>
#include <model_request.h>
#include <cmd_input_def.h>


class ICmdHandler {
	public:
        virtual std::string cmd() = 0;
        virtual bool accessUnauthorized() = 0;
        virtual bool accessUser() = 0;
        virtual bool accessTester() = 0;
        virtual bool accessAdmin() = 0;
        virtual QString description() = 0;
        virtual const std::vector<CmdInputDef> &inputs() = 0;
        virtual QStringList errors() = 0;
        virtual void handle(ModelRequest *pRequest) = 0;
};

#endif // INTERFACES_ICMDHANDLER_H
