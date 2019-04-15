#include <utils_logger.h>
#include <cmd_handlers.h>
#include <employ_ws_server.h>

// ****************************
// **** CmdInputDef
// ****************************

// ---------------------------------------------------------------------

CmdInputDef::CmdInputDef(const std::string &sName, const std::string &sDescription) {
    m_sName = sName;
    m_sDescription = sDescription;
    m_bSettedMinVal = false;
    m_bSettedMaxVal = false;
}

// ---------------------------------------------------------------------

CmdInputDef::CmdInputDef() {
    m_bSettedMinVal = false;
    m_bSettedMaxVal = false;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::optional(){
    m_sRestrict = "optional";
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::required(){
    m_sRestrict = "required";
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::string_(){
    m_sType = std::string(CMD_INPUT_DEF_TYPE_STRING);
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::integer_(){
    m_sType = CMD_INPUT_DEF_TYPE_INTEGER;
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::bool_(){
    m_sType = std::string(CMD_INPUT_DEF_TYPE_BOOL);
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::any_(){
    m_sType = std::string(CMD_INPUT_DEF_TYPE_ANY);
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::description(const std::string& s){
    m_sDescription = s;
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::minval(int minval){
    m_bSettedMinVal = true;
    m_nMinVal = minval;
    return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::maxval(int maxval){
    m_bSettedMaxVal = true;
    m_nMaxVal = maxval;
    return *this;
}

// ---------------------------------------------------------------------

nlohmann::json CmdInputDef::toJson(){
    nlohmann::json obj;
    obj["name"] = m_sName;
    obj["type"] = m_sType;
    obj["restrict"] = m_sRestrict;
    obj["description"] = m_sDescription;
    // TODO validator description
    return obj;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getType(){
    return m_sType;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getType() const{
    return m_sType;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getName(){
    return m_sName;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getName() const {
    return m_sName;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getRestrict(){
    return m_sRestrict;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getRestrict() const{
    return m_sRestrict;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getDescription(){
    return m_sDescription;
}

// ---------------------------------------------------------------------

const std::string &CmdInputDef::getDescription() const{
    return m_sDescription;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isRequired(){
    return m_sRestrict == "required";
}

// ---------------------------------------------------------------------

bool CmdInputDef::isInteger(){
    return m_sType == CMD_INPUT_DEF_TYPE_INTEGER;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isString(){
    return m_sType == CMD_INPUT_DEF_TYPE_STRING;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isBool(){
    return m_sType == CMD_INPUT_DEF_TYPE_BOOL;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isAny() {
    return m_sType == CMD_INPUT_DEF_TYPE_ANY;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isMinVal(){
    return m_bSettedMaxVal;
}

// ---------------------------------------------------------------------

int CmdInputDef::getMinVal(){
    return m_nMinVal;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isMaxVal(){
    return m_bSettedMaxVal;
}

// ---------------------------------------------------------------------

int CmdInputDef::getMaxVal(){
    return m_nMaxVal;
}

// ---------------------------------------------------------------------

const std::vector<ValidatorStringBase *> &CmdInputDef::listOfValidators() {
    return m_vValidatorsString;
}

// ---------------------------------------------------------------------

CmdInputDef &CmdInputDef::addValidator(ValidatorStringBase *pValidatorStringBase) {
    // TODO check type
    m_vValidatorsString.push_back(pValidatorStringBase);
    return *this;
}

// ---------------------------------------------------------------------

// ****************************
// **** CmdHandlerBase
// ****************************

// ---------------------------------------------------------------------

CmdHandlerBase::CmdHandlerBase(const std::string &sCmd, const std::string &sDescription){
    m_sCmd = sCmd;
    m_sDescription = sDescription;
    TAG = "CmdHandlerBase(" + sCmd + ")";

    m_bAccessUnauthorized = false;
    m_bAccessUser = false;
    m_bAccessAdmin = false;

    // can register in global variable
    CmdHandlers::addHandler(sCmd, this);
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::activatedFromVersion(){
    return m_sActivatedFromVersion;
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::deprecatedFromVersion(){
    return m_sDeprecatedFromVersion;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::accessUnauthorized() {
    return m_bAccessUnauthorized;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::accessUser() {
    return m_bAccessUser;
}

// ---------------------------------------------------------------------

bool CmdHandlerBase::accessAdmin() {
    return m_bAccessAdmin;
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::cmd(){
    return m_sCmd;
}

// ---------------------------------------------------------------------

std::string CmdHandlerBase::description(){
    return m_sDescription;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessUnauthorized(bool bAccess){
    m_bAccessUnauthorized = bAccess;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessUser(bool bAccess){
    m_bAccessUser = bAccess;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setAccessAdmin(bool bAccess){
    m_bAccessAdmin = bAccess;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setActivatedFromVersion(const std::string &sActivatedFromVersion){
    m_sActivatedFromVersion = sActivatedFromVersion;
}

// ---------------------------------------------------------------------

void CmdHandlerBase::setDeprecatedFromVersion(const std::string &sDeprecatedFromVersion){
    m_sDeprecatedFromVersion = sDeprecatedFromVersion;
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::requireStringParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pStringDef(sName, sDescription);
    pStringDef.string_().required();
    m_vInputs.push_back(pStringDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::optionalStringParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pStringDef(sName, sDescription);
    pStringDef.string_().optional();
    m_vInputs.push_back(pStringDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::requireIntegerParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pIntegerDef(sName, sDescription);
    pIntegerDef.integer_().required();
    m_vInputs.push_back(pIntegerDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::optionalIntegerParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pIntegerDef(sName, sDescription);
    pIntegerDef.integer_().optional();
    m_vInputs.push_back(pIntegerDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::requireBooleanParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pBooleanDef(sName, sDescription);
    pBooleanDef.bool_().required();
    m_vInputs.push_back(pBooleanDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

CmdInputDef &CmdHandlerBase::optionalBooleanParam(const std::string &sName, const std::string &sDescription) {
    // TODO check duplicates
    CmdInputDef pBooleanDef(sName, sDescription);
    pBooleanDef.bool_().optional();
    m_vInputs.push_back(pBooleanDef);
    return m_vInputs[m_vInputs.size()-1];
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerBase::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

/*
void CmdHandlerBase::success(nlohmann::json jsonResponse){
    EmployWsServer *pEmployWsServer = findEmploy<EmployWsServer>();
    // TODO sendMessageSuccess
    // and remove from ModelRequests
}

// ---------------------------------------------------------------------

void CmdHandlerBase::error(int nCode, const std::string &sErrorMessage){
    EmployWsServer *pEmployWsServer = findEmploy<EmployWsServer>();
    // TODO sendMessageError
    // and remove from ModelRequests

}
// ---------------------------------------------------------------------
*/

// ****************************
// **** CmdHandlers
// ****************************

std::map<std::string, CmdHandlerBase*> *g_pCmdHandlers = NULL;

void CmdHandlers::initGlobalVariables(){
    if(g_pCmdHandlers == NULL){
        // Log::info(std::string(), "Create handlers map");
        g_pCmdHandlers = new std::map<std::string, CmdHandlerBase*>();
    }
}

// ---------------------------------------------------------------------

void CmdHandlers::addHandler(const std::string &sCmd, CmdHandlerBase* pCmdHandler){
    CmdHandlers::initGlobalVariables();
    if(g_pCmdHandlers->count(sCmd)){
        Log::err(sCmd, "Already registered");
    }else{
        g_pCmdHandlers->insert(std::pair<std::string, CmdHandlerBase*>(sCmd,pCmdHandler));
        // Log::info(sCmd, "Registered");
    }
}

// ---------------------------------------------------------------------

CmdHandlerBase * CmdHandlers::findCmdHandler(const std::string &sCmd){
    CmdHandlers::initGlobalVariables();
    CmdHandlerBase *pCmdHandler = NULL;

    if(g_pCmdHandlers->count(sCmd)){
        pCmdHandler = g_pCmdHandlers->at(sCmd);
    }

    if(pCmdHandler == NULL){
        Log::err(sCmd, "Not found");
    }

    return pCmdHandler;
}

