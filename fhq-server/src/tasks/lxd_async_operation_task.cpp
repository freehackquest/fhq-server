//
// Created by sergo on 26.10.18.
//
#include <lxd_async_operation_task.h>
#include <iostream>
#include <QtCore/QThread>
#include <employees.h>


LXDAsyncOperationTask::LXDAsyncOperationTask(void (*func)(const std::string&, std::string &, int &),
                                             const std::string& sName, const std::string& sCMD, WsjcppJsonRpc20Request *pRequest) {
    TAG = "LXDAsyncOperationTask";
    m_func = func;
    m_pRequestClient = pRequest->getWebSocketClient();
    m_sName = sName;
    m_sCMD = sCMD;
    m_sId = pRequest->getId();
}

LXDAsyncOperationTask::~LXDAsyncOperationTask() = default;

void LXDAsyncOperationTask::run() {
    nlohmann::json jsonResponse;
    jsonResponse["cmd"] = m_sCMD;
    jsonResponse["m"] = m_sId;
    std::string sError;
    int nErrorCode = 500;

    auto *pWsServer = findWsjcppEmploy<EmployServer>();
    auto jsonPrepare = jsonResponse;
    jsonPrepare["result"] = "Operation began";
    pWsServer->sendToOne(m_pRequestClient, jsonPrepare);

    m_func(m_sName, sError, nErrorCode);

    if (sError.empty()) {
        jsonResponse["result"] = "DONE";
    } else {
        jsonResponse["result"] = "FAIL";
        jsonResponse["error"] = sError;
        jsonResponse["code"] = nErrorCode;
    }

    pWsServer->sendToOne(m_pRequestClient, jsonResponse);
}
