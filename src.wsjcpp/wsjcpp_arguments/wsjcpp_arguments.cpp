#include "wsjcpp_arguments.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// WsjcppArgumentSingle

WsjcppArgumentSingle::WsjcppArgumentSingle(const std::string &sName, const std::string &sDescription) {
    TAG = "WsjcppArgumentSingle-" + sName;
    m_sName = sName;
    m_sDescription = sDescription;
}

// ---------------------------------------------------------------------

std::string WsjcppArgumentSingle::getName() {
    return m_sName;
}

// ---------------------------------------------------------------------

std::string WsjcppArgumentSingle::getDescription() {
    return m_sDescription;
}

// ---------------------------------------------------------------------
// WsjcppArgumentParameter

WsjcppArgumentParameter::WsjcppArgumentParameter(
    const std::string &sName,
    const std::string &sValueName, 
    const std::string &sDescription
) {
    TAG = "WsjcppArgumentParameter-" + sName;
    m_sName = sName;
    m_sValueName = sValueName;
    m_sDescription = sDescription;
}

// ---------------------------------------------------------------------

std::string WsjcppArgumentParameter::getName() {
    return m_sName;
}

// ---------------------------------------------------------------------

std::string WsjcppArgumentParameter::getValueName() {
    return m_sValueName;
}

// ---------------------------------------------------------------------

std::string WsjcppArgumentParameter::getDescription() {
    return m_sDescription;
}

// ---------------------------------------------------------------------

std::string WsjcppArgumentParameter::getValue() {
    return m_sValue;
}

// ---------------------------------------------------------------------

void WsjcppArgumentParameter::setValue(const std::string &sValue) {
    m_sValue = sValue;
}

// ---------------------------------------------------------------------
// WsjcppArgumentProcessor

WsjcppArgumentProcessor::WsjcppArgumentProcessor(
    const std::vector<std::string> &vNames, 
    const std::string &sShortDescription,
    const std::string &sDescription
) {
    if (vNames.size() < 1) {
        WsjcppLog::throw_err(TAG, "Names must have 1 or more values for '" + sDescription + "'");
    }
    m_vNames = vNames;
    TAG = "WsjcppArgumentProcessor-" + m_vNames[0];
    m_sShortDescription = sShortDescription;
    m_sDescription = sDescription;
}

// ---------------------------------------------------------------------

const std::vector<std::string> &WsjcppArgumentProcessor::getNames() {
    return m_vNames;
}

// ---------------------------------------------------------------------

std::string WsjcppArgumentProcessor::getNamesAsString() {
    std::string sRet;
    for (int i = 0; i < m_vNames.size(); i++) {
        if (sRet.size() > 0) {
            sRet += ", ";
        }
        sRet += m_vNames[i];
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WsjcppArgumentProcessor::getShortDescription() {
    return m_sShortDescription;
}

// ---------------------------------------------------------------------

std::string WsjcppArgumentProcessor::getDescription() {
    return m_sDescription;
}

// ---------------------------------------------------------------------

void WsjcppArgumentProcessor::registryProcessor(WsjcppArgumentProcessor *p) {
    for (int i = 0; i < m_vNames.size(); i++) {
        std::string sName = m_vNames[i];
        if (hasRegisteredArgumentName(sName)) {
            WsjcppLog::throw_err(TAG, "Argument Name '" + sName + "' already registered");
        }
    }
    m_vProcessors.push_back(p);
}

// ---------------------------------------------------------------------

void WsjcppArgumentProcessor::registryExample(const std::string &sExample) {
    m_vExamples.push_back(sExample);
}

// ---------------------------------------------------------------------

void WsjcppArgumentProcessor::registrySingleArgument(const std::string &sArgumentName, const std::string &sDescription) {
    if (hasRegisteredArgumentName(sArgumentName)) {
        WsjcppLog::throw_err(TAG, "Argument Name '" + sArgumentName + "' already registered");
    }
    m_vSingleArguments.push_back(new WsjcppArgumentSingle(sArgumentName, sDescription));
}

// ---------------------------------------------------------------------

void WsjcppArgumentProcessor::registryParameterArgument(
    const std::string &sArgumentName,
    const std::string &sArgumentValueName, 
    const std::string &sDescription
) {
    if (hasRegisteredArgumentName(sArgumentName)) {
        WsjcppLog::throw_err(TAG, "Argument Name '" + sArgumentName + "' already registered");
    }
    m_vParameterArguments.push_back(new WsjcppArgumentParameter(sArgumentName, sArgumentValueName, sDescription));
}

// ---------------------------------------------------------------------

WsjcppArgumentSingle *WsjcppArgumentProcessor::findRegisteredSingleArgument(const std::string &sArgumentName) {
    WsjcppArgumentSingle *pRet = nullptr;
    for (int i = 0; i < m_vSingleArguments.size(); i++) {
        if (m_vSingleArguments[i]->getName() == sArgumentName) {
            if (pRet == nullptr) {
                pRet = m_vSingleArguments[i];
            } else {
                WsjcppLog::throw_err(TAG, "Single argument '" + sArgumentName + "' already exists");
            }
        }
    }
    return pRet;
}

// ---------------------------------------------------------------------

WsjcppArgumentParameter *WsjcppArgumentProcessor::findRegisteredParameterArgument(const std::string &sArgumentName) {
    WsjcppArgumentParameter *pRet = nullptr;
    for (int i = 0; i < m_vParameterArguments.size(); i++) {
        if (m_vParameterArguments[i]->getName() == sArgumentName) {
            if (pRet == nullptr) {
                pRet = m_vParameterArguments[i];
            } else {
                WsjcppLog::throw_err(TAG, "Single argument '" + sArgumentName + "' already exists");
            }
        }
    }
    return pRet;
}


// ---------------------------------------------------------------------

WsjcppArgumentProcessor *WsjcppArgumentProcessor::findRegisteredProcessor(const std::string &sArgumentName) {
    WsjcppArgumentProcessor *pRet = nullptr;
    for (int i = 0; i < m_vProcessors.size(); i++) {
        std::vector<std::string> vNames = m_vProcessors[i]->getNames();
        for (int n = 0; n < vNames.size(); n++) {
            if (vNames[n] == sArgumentName) {
                if (pRet == nullptr) {
                    pRet = m_vProcessors[i];
                } else {
                    WsjcppLog::throw_err(TAG, "Several processors can handle this arguments");
                }
            }
        }
    }
    return pRet;
}

// ---------------------------------------------------------------------

bool WsjcppArgumentProcessor::hasRegisteredArgumentName(const std::string &sArgumentName) {
    for (int i = 0; i < m_vParameterArguments.size(); i++) {
        if (m_vParameterArguments[i]->getName() == sArgumentName) {
            return true;
        }
    }

    for (int i = 0; i < m_vProcessors.size(); i++) {
        std::vector<std::string> vNames = m_vProcessors[i]->getNames();
        for (int n = 0; n < vNames.size(); n++) {
            if (vNames[n] == sArgumentName) {
                return true;
            }
        }
    }

    for (int i = 0; i < m_vSingleArguments.size(); i++) {
        if (m_vSingleArguments[i]->getName() == sArgumentName) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

int WsjcppArgumentProcessor::help(
    const std::vector<std::string> &vRoutes, 
    const std::vector<std::string> &vSubParams
) {
    std::string sPrefix = "     ";
    int nParams = 0;
    
    std::string sSynopsis = "";
    std::string sManName = "";

    for (int i = 0; i < vRoutes.size(); i++) {
        sSynopsis += vRoutes[i];
        sManName += vRoutes[i];
        if (i < vRoutes.size() - 1) {
            sSynopsis += " ";
            sManName += "-";
        }
    }

    std::cout
        << std::endl
        << "NAME:" << std::endl
        << "    " << sManName << " - " << this->getShortDescription() << std::endl
        << std::endl
        << "SYNOPSIS:" << std::endl
        << "    " << sSynopsis << " [<options>...]" << std::endl
        << std::endl
        << "DESCRIPTION:" << std::endl
        << "     " << this->getDescription() << std::endl; // TODO 66 simbols in line
    
    if (m_vExamples.size() > 0) {
        for (int ei = 0; ei < m_vExamples.size(); ei++) {
            std::cout << "     example " << ei << ": '" << m_vExamples[ei] << "'" << std::endl;
        }
    }

    std::cout
        << std::endl;

    if (m_vSingleArguments.size() > 0 || m_vParameterArguments.size() || m_vProcessors.size() > 0) {
        std::cout << "OPTIONS:" << std::endl;

        for (int i = 0; i < m_vSingleArguments.size(); i++) {
            WsjcppArgumentSingle *pArg = m_vSingleArguments[i];
            std::cout 
                << "     " << pArg->getName() 
                << std::endl
                << "          " << pArg->getDescription()
                << std::endl
                << std::endl;
        }

        for (int i = 0; i < m_vParameterArguments.size(); i++) {
            WsjcppArgumentParameter *pArg = m_vParameterArguments[i];
            std::cout 
                << "     " << pArg->getName() << " " << pArg->getValueName()
                << std::endl
                << "          " << pArg->getDescription()
                << std::endl
                << std::endl;
        }

        for (int i = 0; i < m_vProcessors.size(); i++) {
            WsjcppArgumentProcessor *pProc = m_vProcessors[i];
            std::cout << "     " << pProc->getNamesAsString();

            if (pProc->hasMoreOptions()) {
                std::cout 
                    << " [<options>...]"
                    << std::endl
                    << "          Subcommand. Try help for more. " << pProc->getDescription()
                    << std::endl
                ;
            } else {
                std::cout 
                    << std::endl
                    << "          " << pProc->getDescription()
                    << std::endl
                ;
            }
            std::cout << std::endl;            
        }
    }
    return 0;
}

// ---------------------------------------------------------------------

bool WsjcppArgumentProcessor::hasMoreOptions() {
    return m_vProcessors.size() > 0
        || m_vSingleArguments.size() > 0
        || m_vParameterArguments.size() > 0;
}

// ---------------------------------------------------------------------

bool WsjcppArgumentProcessor::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::throw_err(TAG, "No support single argument '" + sArgumentName + "'");
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppArgumentProcessor::applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName, const std::string &sValue) {
    WsjcppLog::err(TAG, "No support parameter argument '" + sArgumentName + "' for '" + getNamesAsString() + "'");
    return false;
}

// ---------------------------------------------------------------------

int WsjcppArgumentProcessor::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::err(TAG, "Processor '" + getNamesAsString() + "' has not implementation");
    return -10;
}

// ---------------------------------------------------------------------
// WsjcppArguments

WsjcppArguments::WsjcppArguments(int argc, const char* argv[], WsjcppArgumentProcessor *pRoot) {
    TAG = "WsjcppArguments";
    for (int i = 0; i < argc; i++) {
        m_vArguments.push_back(std::string(argv[i]));
    }
    m_sProgramName = m_vArguments[0];
    m_vArguments.erase(m_vArguments.begin());
    m_pRoot = pRoot;
    m_bEnablePrintAutoHelp = true;
}

// ---------------------------------------------------------------------

WsjcppArguments::~WsjcppArguments() {
    for (int i = 0; i < m_vProcessors.size(); i++) {
        delete m_vProcessors[i];
    }
}

// ---------------------------------------------------------------------

void WsjcppArguments::enablePrintAutoHelp(bool bEnablePrintAutoHelp) {
    m_bEnablePrintAutoHelp = bEnablePrintAutoHelp;
}

// ---------------------------------------------------------------------

int WsjcppArguments::exec() {
    if (m_pRoot == nullptr) {
        WsjcppLog::throw_err(TAG, "Root could not be nullptr");
    }
    std::vector<std::string> vArgs(m_vArguments);
    std::vector<std::string> vRoutes;
    vRoutes.push_back(m_sProgramName);
    return this->recursiveExec(m_pRoot, vRoutes, vArgs);
}

// ---------------------------------------------------------------------

int WsjcppArguments::recursiveExec(
    WsjcppArgumentProcessor *pArgumentProcessor, 
    std::vector<std::string> &vRoutes,
    std::vector<std::string> &vSubArguments
) {
    
    std::vector<WsjcppArgumentSingle *> vSingleArguments;
    std::vector<WsjcppArgumentParameter *> vParameterArguments;
    vSubArguments = extractSingleAndParameterArguments(pArgumentProcessor, vSubArguments, vSingleArguments, vParameterArguments);

    // apply single arguments
    for (int i = 0; i < vSingleArguments.size(); i++) {
        WsjcppArgumentSingle *p = vSingleArguments[i];
        if (!pArgumentProcessor->applySingleArgument(m_sProgramName, p->getName())) {
            WsjcppLog::err(TAG, "Could not apply single argument '" + p->getName() + "' ");
            return -1;
        }
    }

    // apply parameter arguments
    for (int i = 0; i < vParameterArguments.size(); i++) {
        WsjcppArgumentParameter *p = vParameterArguments[i];
        if (!pArgumentProcessor->applyParameterArgument(m_sProgramName, p->getName(), p->getValue())) {
            WsjcppLog::err(TAG, "Could not apply parameter argument '" + p->getName() + "' for '" + pArgumentProcessor->getNamesAsString() + "'");
            return -1;
        }
    }

    if (vSubArguments.size() > 0) {
        WsjcppArgumentProcessor *pNextProcessor = pArgumentProcessor->findRegisteredProcessor(vSubArguments[0]);
        if (pNextProcessor != nullptr) {
            vRoutes.push_back(vSubArguments[0]);
            vSubArguments.erase(vSubArguments.begin());
            return this->recursiveExec(pNextProcessor, vRoutes, vSubArguments);
        }
    }
    
    if (vSubArguments.size() > 0 && vSubArguments[0] == "help") {
        return pArgumentProcessor->help(vRoutes, vSubArguments);
    }
    
    int nResult = pArgumentProcessor->exec(vRoutes, vSubArguments);
    if (nResult == -10 && m_bEnablePrintAutoHelp) {
        pArgumentProcessor->help(vRoutes, vSubArguments);
    }
    return nResult;
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppArguments::extractSingleAndParameterArguments(
    WsjcppArgumentProcessor *pArgumentProcessor, 
    const std::vector<std::string> &vArguments,
    std::vector<WsjcppArgumentSingle *> &vSingleArguments,
    std::vector<WsjcppArgumentParameter *> &vParameterArguments
) {
    std::vector<std::string> vArgs(vArguments);
    bool bFound = true;
    while (bFound) {
        bFound = false;
        if (vArgs.size() > 0) {
            std::string sFirst = vArgs[0];
            WsjcppArgumentSingle *pSingle = pArgumentProcessor->findRegisteredSingleArgument(sFirst);
            WsjcppArgumentParameter *pParameter = pArgumentProcessor->findRegisteredParameterArgument(sFirst);
            if (pSingle != nullptr) {
                vArgs.erase(vArgs.begin());
                bFound = true;
                vSingleArguments.push_back(pSingle);
            } else if (pParameter != nullptr) {
                bFound = true;
                vParameterArguments.push_back(pParameter);
                vArgs.erase(vArgs.begin());
                if (vArgs.empty()) {
                    WsjcppLog::throw_err(TAG, "Expected value for '" + pParameter->getName() + "'");
                } else {
                    pParameter->setValue(vArgs[0]);
                    vArgs.erase(vArgs.begin());
                }
            }
        }
    }
    return vArgs;
}

// ---------------------------------------------------------------------