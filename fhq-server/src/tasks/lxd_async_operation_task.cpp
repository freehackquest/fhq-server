//
// Created by sergo on 26.10.18.
//
#include <lxd_async_operation_task.h>
#include <iostream>
#include <QtCore/QThread>
#include <wsjcpp_employees.h>


LXDAsyncOperationTask::LXDAsyncOperationTask(void (*func)(const std::string&, std::string &, int &),
                                             const std::string& sName, const std::string& sCMD, ModelRequest *pRequest) {
    TAG = "LXDAsyncOperationTask";
    m_func = func;
    m_pRequestClient = pRequest->client();
    m_sName = sName;
    m_sCMD = sCMD;
    m_sM = pRequest->m();
}

LXDAsyncOperationTask::~LXDAsyncOperationTask() = default;

void LXDAsyncOperationTask::run() {
    nlohmann::json jsonResponse;
    jsonResponse["cmd"] = m_sCMD;
    jsonResponse["m"] = m_sM;
    std::string sError;
    int nErrorCode = 500;

    auto *pWsServer = findEmploy<EmployServer>();
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
