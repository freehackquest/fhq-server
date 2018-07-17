#include <model_game.h>
#include <iostream>

// ---------------------------------------------------------------------

ModelGame::ModelGame(){
    TAG = "ModelLeak";
    m_nLocalId = 0;
    m_sUuid = "";
    m_sName = "";
    m_sDescription = "";
    m_sState = "";
    m_sForm = "";
    m_sType = "";
    m_sDateStart = "";
    m_sDateStop = "";
    m_sDateRestart = "";
    m_sOrganizators = "";
}

// ---------------------------------------------------------------------

int ModelGame::localId(){
    return m_nLocalId;
}

// ---------------------------------------------------------------------

void ModelGame::setLocalId(int nLocalId){
    m_nLocalId = nLocalId;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::uuid(){
	return m_sUuid;
}

// ---------------------------------------------------------------------

void ModelGame::setUuid(std::string sUuid){
	m_sUuid = sUuid;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::name(){
    return m_sName;
}

// ---------------------------------------------------------------------

void ModelGame::setName(std::string sName){
    m_sName = sName;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::description(){
    return m_sDescription;
}

// ---------------------------------------------------------------------

void ModelGame::setDescription(std::string sDescription){
    m_sDescription = sDescription;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::state(){
    return m_sState;
}

// ---------------------------------------------------------------------

void ModelGame::setState(std::string sState){
    m_sState = sState;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::form(){
    return m_sForm;
}

// ---------------------------------------------------------------------

void ModelGame::setForm(std::string sForm){
    m_sForm = sForm;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::type(){
    return m_sType;
}

// ---------------------------------------------------------------------

void ModelGame::setType(std::string sType){
    m_sType = sType;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::dateStart(){
    return m_sDateStart;
}

// ---------------------------------------------------------------------

void ModelGame::setDateStart(std::string sDateStart){
    m_sDateStart = sDateStart;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::dateStop(){
    return m_sDateStop;
}

// ---------------------------------------------------------------------

void ModelGame::setDateStop(std::string sDateStop){
    m_sDateStop = sDateStop;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::dateRestart(){
    return m_sDateRestart;
}

// ---------------------------------------------------------------------

void ModelGame::setDateRestart(std::string sDateRestart){
    m_sDateRestart = sDateRestart;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::organizators(){
    return m_sOrganizators;
}

// ---------------------------------------------------------------------

void ModelGame::setOrganizators(std::string sOrganizators){
    m_sOrganizators = sOrganizators;
}

// ---------------------------------------------------------------------

int ModelGame::maxScore(){
    return m_nMaxScore;
}

// ---------------------------------------------------------------------

void ModelGame::setMaxScore(int nMaxScore){
    m_nMaxScore = nMaxScore;
}

// ---------------------------------------------------------------------

nlohmann::json ModelGame::toJson(){
    nlohmann::json jsonGame;
    jsonGame["local_id"] = m_nLocalId; // deprecated
    jsonGame["uuid"] = m_sUuid;
    jsonGame["name"] = m_sName;
    jsonGame["description"] = m_sDescription;
    jsonGame["state"] = m_sState;
    jsonGame["type"] = m_sType;
    jsonGame["date_start"] = m_sDateStart;
    jsonGame["date_stop"] = m_sDateStop;
    jsonGame["date_restart"] = m_sDateRestart;
    jsonGame["organizators"] = m_sOrganizators;
    jsonGame["maxscore"] = m_nMaxScore;
    return jsonGame;
}

// ---------------------------------------------------------------------

void ModelGame::fillFrom(nlohmann::json &jsonGame){
    // TODO trim
    if(jsonGame["name"].is_string()){
        m_sName = jsonGame["name"];
    }

    // uuid
    if(jsonGame["uuid"].is_string()){
        m_sUuid = jsonGame["uuid"];
    }

    // TODO trim
    if(jsonGame["description"].is_string()){
        m_sDescription = jsonGame["description"];
    }

    // TODO trim
    if(jsonGame["state"].is_string()){
        m_sState = jsonGame["state"];
    }

    // TODO trim
    if(jsonGame["type"].is_string()){
        m_sType = jsonGame["type"];
    }

    // TODO trim
    if(jsonGame["date_start"].is_string()){
        m_sDateStart = jsonGame["date_start"];
    }

    // TODO trim
    if(jsonGame["date_stop"].is_string()){
        m_sDateStop = jsonGame["date_stop"];
    }

    // TODO trim
    if(jsonGame["date_restart"].is_string()){
        m_sDateRestart = jsonGame["date_restart"];
    }

    // TODO trim
    if(jsonGame["organizators"].is_string()){
        m_sOrganizators = jsonGame["organizators"];
    }
}

// ---------------------------------------------------------------------
