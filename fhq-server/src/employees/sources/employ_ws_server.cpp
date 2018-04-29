#include <employ_ws_server.h>
#include <employ_server_config.h>
#include <employ_server_info.h>
#include <log.h>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QRegularExpression>

REGISTRY_EMPLOY(EmployWsServer)

// ---------------------------------------------------------------------

EmployWsServer::EmployWsServer()
    : EmployBase(EmployWsServer::name(), {"start_ws_server", EmployServerConfig::name(), EmployServerInfo::name()}) {
	TAG = EmployWsServer::name();
}

// ---------------------------------------------------------------------

bool EmployWsServer::init(){

    return true;
}

// ---------------------------------------------------------------------

bool EmployWsServer::validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler, QJsonObject &jsonRequest){
    const std::vector<CmdInputDef> vInputs = pCmdHandler->inputs();
    for(unsigned int i = 0; i < vInputs.size(); i++){
		CmdInputDef inDef = vInputs[i];
        QString sParamName = QString(inDef.getName().c_str());
        if(inDef.isRequired() && !jsonRequest.contains(sParamName)){
			error = Errors::ParamExpected(sParamName);
			return false;
		}
		
        if(jsonRequest.contains(sParamName)){
			if(inDef.isInteger()){
                QJsonValueRef vParam = jsonRequest[sParamName];
				if(!vParam.isDouble()){
					error = Errors::ParamMustBeInteger(sParamName);
					return false;
				}
                int val = jsonRequest[sParamName].toInt();
				if(inDef.isMinVal() && val < inDef.getMinVal()){
					error = Errors::ParamMustBeMoreThen(sParamName, inDef.getMinVal());
					return false;
				}
				if(inDef.isMaxVal() && val > inDef.getMaxVal()){
					error = Errors::ParamMustBeLessThen(sParamName, inDef.getMaxVal());
					return false;
				}
			}
			
			if(inDef.isEnum()){
                QString val = jsonRequest[sParamName].toString().trimmed();
				QStringList eList = inDef.getEnumList();
				if(!eList.contains(val)){
					error = Errors::ParamExpectedValueOneFrom(sParamName,eList);
					return false;
				}
			}
			
			if(inDef.isUUID()){
                QString val = jsonRequest[sParamName].toString();
                QRegularExpression rx("[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}");
                if(!rx.isValid()){
					Log::err(TAG, "validateInputParameters, " + rx.errorString().toStdString());
				}
                if(!rx.match(val).hasMatch()){
					error = Errors::ParamExpectedUUID(sParamName);
					return false;
				}
			}

            if(inDef.isEmail()){
                QString val = jsonRequest[sParamName].toString();
                QRegularExpression rx("^[0-9a-zA-Z]{1}[0-9a-zA-Z-._]*[0-9a-zA-Z]{1}@[0-9a-zA-Z]{1}[-.0-9a-zA-Z]*[0-9a-zA-Z]{1}\\.[a-zA-Z]{2,6}$");
                if(!rx.isValid()){
                    Log::err(TAG, "validateInputParameters, " + rx.errorString().toStdString());
                }
                if(!rx.match(val).hasMatch()){
                    error = Errors::ParamMustBeEmail(sParamName);
                    return false;
                }
            }
		}
	}
	return true;
}
