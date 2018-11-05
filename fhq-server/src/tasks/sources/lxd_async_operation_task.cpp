//
// Created by sergo on 26.10.18.
//
#include <lxd_async_operation_task.h>


LXDAsyncOperationTask::LXDAsyncOperationTask(void (*func)(std::string, std::string &, int &),
                                             std::string sName, std::string sCMD, ModelRequest *pRequest) {
    TAG = "LXDAsyncOperationTask";
    m_func = func;
    m_pRequest = pRequest;
    m_sName = std::move(sName);
    m_sCMD = std::move(sCMD);
}

LXDAsyncOperationTask::~LXDAsyncOperationTask() = default;

void LXDAsyncOperationTask::run() {
    QJsonObject jsonResponse;
    std::string sError;
    int nErrorCode = 500;
    m_func(m_sName, sError, nErrorCode);

    if (sError.empty())
        m_pRequest->sendMessageSuccess(m_sCMD, jsonResponse);
    else
        m_pRequest->sendMessageError(m_sCMD, Error(nErrorCode, sError));

}
