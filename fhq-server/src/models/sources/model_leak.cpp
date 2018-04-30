#include <model_leak.h>
#include <iostream>

// ---------------------------------------------------------------------

ModelLeak::ModelLeak(){
    TAG = "ModelLeak";
}

// ---------------------------------------------------------------------

int ModelLeak::id(){
	return m_nId;
}

// ---------------------------------------------------------------------

void ModelLeak::setId(int nId){
	m_nId = nId;
}

// ---------------------------------------------------------------------

const std::string &ModelLeak::uuid(){
	return m_sUuid;
}

// ---------------------------------------------------------------------

void ModelLeak::setUuid(std::string sUuid){
	m_sUuid = sUuid;
}

// ---------------------------------------------------------------------

int ModelLeak::gameId(){
	return m_nGameId;
}

// ---------------------------------------------------------------------

void ModelLeak::setGameId(int nGameId){
	m_nGameId = nGameId;
}

// ---------------------------------------------------------------------

const std::string &ModelLeak::name(){
	return m_sName;
}

// ---------------------------------------------------------------------

void ModelLeak::setName(std::string sName){
	m_sName = sName;
}

// ---------------------------------------------------------------------

const std::string &ModelLeak::content(){
	return m_sContent;
}

// ---------------------------------------------------------------------

void ModelLeak::setContent(std::string sContent){
	m_sContent = sContent;
}

// ---------------------------------------------------------------------

int ModelLeak::score(){
	return m_nScore;
}

// ---------------------------------------------------------------------

void ModelLeak::setScore(int nScore){
	m_nScore = nScore;
}

// ---------------------------------------------------------------------

const std::string &ModelLeak::created(){
	return m_sCreated;
}

// ---------------------------------------------------------------------

void ModelLeak::setCreated(std::string sCreated){
	m_sCreated = sCreated;
}

// ---------------------------------------------------------------------

const std::string &ModelLeak::updated(){
	return m_sUpdated;
}

// ---------------------------------------------------------------------

void ModelLeak::setUpdated(std::string sUpdated){
	m_sUpdated = sUpdated;
}

// ---------------------------------------------------------------------

int ModelLeak::sold(){
	return m_nSold;
}

// ---------------------------------------------------------------------

void ModelLeak::setSold(int nSold){
	m_nSold = nSold;
}

// ---------------------------------------------------------------------

nlohmann::json toJson(){
	nlohmann::json jsonLeak;
	// TODO fill jsonLeak
	return jsonLeak;
}
