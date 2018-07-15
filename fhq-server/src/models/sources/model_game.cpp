#include <model_game.h>
#include <iostream>

// ---------------------------------------------------------------------

ModelGame::ModelGame(){
    TAG = "ModelLeak";
    m_nLocalId = 0;
    m_sUuid = "";
    m_sName = "";
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

nlohmann::json ModelGame::toJson(){
    nlohmann::json jsonGame;
    jsonGame["local_id"] = m_nLocalId; // deprecated
    jsonGame["uuid"] = m_sUuid;
    jsonGame["name"] = m_sName;
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
}

// ---------------------------------------------------------------------
