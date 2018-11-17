//
// Created by sergo on 26.10.18.
//
#include <lxd_async_operation_task.h>
#include <iostream>
#include <QtCore/QThread>
#include <include/employ_ws_server.h>


LXDAsyncOperationTask::LXDAsyncOperationTask(void (*func)(std::string, std::string &, int &),
                                             std::string sName, std::string sCMD, ModelRequest *pRequest) {
    TAG = "LXDAsyncOperationTask";
    m_func = func;
    m_pRequest = pRequest->client();
    m_sName = std::move(sName);
    m_sCMD = std::move(sCMD);
}

LXDAsyncOperationTask::~LXDAsyncOperationTask() = default;

void LXDAsyncOperationTask::run() {
    nlohmann::json jsonResponse;
    std::string sError;
    int nErrorCode = 500;
    m_func(m_sName, sError, nErrorCode);

    if (sError.empty()) {
        jsonResponse = R"({"status": "DONE", "cmd": "lxd_containers", "m": "2323")";
    } else {
        jsonResponse = R"({"status": 500, "cmd": "lxd_containers", "m": "2323"})"_json;
    }
//    else
//        m_pRequest->sendMessageError(m_sCMD, Error(nErrorCode, sError));
    auto *pWsServer = findEmploy<EmployWsServer>();
    pWsServer->sendToOne(m_pRequest, jsonResponse);
}
