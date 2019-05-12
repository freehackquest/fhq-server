//
// Created by sergo on 26.10.18.
//

#ifndef FHQ_SERVER_LXDASYNCOPERATIONTASK_H
#define FHQ_SERVER_LXDASYNCOPERATIONTASK_H

#include <QRunnable>
#include <cmd_handlers.h>

class LXDAsyncOperationTask : public QRunnable {

    void run() override;

public:
    LXDAsyncOperationTask(void (*func)(const std::string&, std::string &, int &),
                          const std::string& sName, const std::string& sCMD, ModelRequest *pRequest);

    ~LXDAsyncOperationTask() override;

private:
    void (*m_func)(const std::string&, std::string &, int &);

    std::string m_sCMD;
    std::string TAG;
    std::string m_sName;
    std::string m_sM;
    QWebSocket *m_pRequestClient;
};


#endif //FHQ_SERVER_LXDASYNCOPERATIONTASK_H
