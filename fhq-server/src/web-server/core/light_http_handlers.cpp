#include "light_http_handlers.h"
#include <logger.h>
#include <algorithm>

// ---------------------------------------------------------------------

LightHttpHandlers::LightHttpHandlers() {
    TAG = "LightHttpHandlers";
}

// ---------------------------------------------------------------------

void LightHttpHandlers::add(LightHttpHandlerBase *pHandler) {
    m_pHandlers.push_back(pHandler);
}

// ---------------------------------------------------------------------

void LightHttpHandlers::remove(const std::string &sName) {
    std::vector<LightHttpHandlerBase *>::iterator it = m_pHandlers.begin();
    for (it = m_pHandlers.end(); it != m_pHandlers.begin(); it--) {
        LightHttpHandlerBase *pHandler = *it;
        if (pHandler->name() == sName) {
            m_pHandlers.erase(it);
        }
    }
}

// ---------------------------------------------------------------------

bool LightHttpHandlers::find(const std::string &sName, LightHttpHandlerBase *pHandler) {
    std::vector<LightHttpHandlerBase *>::iterator it = m_pHandlers.begin();
    for (it = m_pHandlers.begin(); it != m_pHandlers.end(); ++it) {
        LightHttpHandlerBase *p = *it;
        if (p->name() == sName) {
            pHandler = p;
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

bool LightHttpHandlers::handle(const std::string &sWorkerId, LightHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;

    for (int i = 0; i < m_pHandlers.size(); i++) {
        if (m_pHandlers[i]->canHandle(sWorkerId, pRequest)) {
            if (m_pHandlers[i]->handle(sWorkerId, pRequest)) {
                return true;
            } else {
                Log::warn("LightHttpHandlers", m_pHandlers[i]->name() + " - could not handle request '" + pRequest->requestPath() + "'");
            }
        }
    }
    return false;
}