#include "light_http_handler_base.h"
#include <logger.h>
#include <algorithm>

// ---------------------------------------------------------------------

LightHttpHandlerBase::LightHttpHandlerBase(const std::string &sName) {
    m_sName = sName;
}

// ---------------------------------------------------------------------

const std::string &LightHttpHandlerBase::name() {
    return m_sName;
}

// ---------------------------------------------------------------------
