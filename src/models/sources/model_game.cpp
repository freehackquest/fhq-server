#include <model_game.h>
#include <fallen.h>
#include <iostream>
#include <algorithm>

// ---------------------------------------------------------------------

ModelGame::ModelGame() {
    TAG = "ModelGame";
    m_nLocalId = 0;
    m_nMaxScore = 0;
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
    m_pValidatorGameType = new ValidatorGameType();
    m_pValidatorGameState = new ValidatorGameState();
    m_pValidatorGameForm = new ValidatorGameForm();
}

// ---------------------------------------------------------------------

int ModelGame::localId() const {
    return m_nLocalId;
}

// ---------------------------------------------------------------------

void ModelGame::setLocalId(int nLocalId) {
    m_nLocalId = nLocalId;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::uuid() const {
    return m_sUuid;
}

// ---------------------------------------------------------------------

void ModelGame::setUuid(std::string sUuid) {
    m_sUuid = sUuid;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::name() const {
    return m_sName;
}

// ---------------------------------------------------------------------

void ModelGame::setName(std::string sName) {
    m_sName = sName;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::description() const {
    return m_sDescription;
}

// ---------------------------------------------------------------------

void ModelGame::setDescription(std::string sDescription) {
    m_sDescription = sDescription;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::state() const {
    return m_sState;
}

// ---------------------------------------------------------------------

void ModelGame::setState(const std::string &sState) {
    std::string sError;
    if (!m_pValidatorGameState->isValid(sState, sError)) {
        WsjcppLog::err(TAG, "Game state unknown:" + sError);
    }
    m_sState = sState;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::form() const {
    return m_sForm;
}

// ---------------------------------------------------------------------

void ModelGame::setForm(std::string sForm) {
    std::string sError;
    if (!m_pValidatorGameForm->isValid(sForm, sError)) {
        WsjcppLog::err(TAG, "Game form unknown: '" + sForm + "' " + sError);
    }
    m_sForm = sForm;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::type() const {
    return m_sType;
}

// ---------------------------------------------------------------------

void ModelGame::setType(std::string sType) {
    std::string sError;
    if (!m_pValidatorGameType->isValid(sType, sError)) {
        WsjcppLog::err(TAG, "Game type unknown:" + sError);
    }
    m_sType = sType;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::dateStart() const {
    return m_sDateStart;
}

// ---------------------------------------------------------------------

void ModelGame::setDateStart(std::string sDateStart) {
    m_sDateStart = sDateStart;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::dateStop() const {
    return m_sDateStop;
}

// ---------------------------------------------------------------------

void ModelGame::setDateStop(std::string sDateStop) {
    m_sDateStop = sDateStop;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::dateRestart() const {
    return m_sDateRestart;
}

// ---------------------------------------------------------------------

void ModelGame::setDateRestart(std::string sDateRestart) {
    m_sDateRestart = sDateRestart;
}

// ---------------------------------------------------------------------

const std::string &ModelGame::organizators() const {
    return m_sOrganizators;
}

// ---------------------------------------------------------------------

void ModelGame::setOrganizators(std::string sOrganizators) {
    m_sOrganizators = sOrganizators;
}

// ---------------------------------------------------------------------

int ModelGame::maxScore() const{
    return m_nMaxScore;
}

// ---------------------------------------------------------------------

void ModelGame::setMaxScore(int nMaxScore) {
    m_nMaxScore = nMaxScore;
}

// ---------------------------------------------------------------------

void ModelGame::copy(const ModelGame &modelGame) {
    this->setLocalId(modelGame.localId());
    this->setUuid(modelGame.uuid());
    this->setName(modelGame.name());
    this->setDescription(modelGame.description());
    this->setState(modelGame.state());
    this->setForm(modelGame.form());
    this->setType(modelGame.type());
    this->setDateStart(modelGame.dateStart());
    this->setDateStop(modelGame.dateStop());
    this->setDateRestart(modelGame.dateRestart());
    this->setOrganizators(modelGame.organizators());
    this->setMaxScore(modelGame.maxScore());
}

// ---------------------------------------------------------------------

ModelGame *ModelGame::clone() const {
    ModelGame *pModel = new ModelGame();
    pModel->setLocalId(this->localId());
    pModel->setUuid(this->uuid());
    pModel->setName(this->name());
    pModel->setDescription(this->description());
    pModel->setState(this->state());
    pModel->setForm(this->form());
    pModel->setType(this->type());
    pModel->setDateStart(this->dateStart());
    pModel->setDateStop(this->dateStop());
    pModel->setDateRestart(this->dateRestart());
    pModel->setOrganizators(this->organizators());
    pModel->setMaxScore(this->maxScore());
    return pModel;
}

// ---------------------------------------------------------------------

nlohmann::json ModelGame::toJson() {
    nlohmann::json jsonGame;
    jsonGame["local_id"] = m_nLocalId; // deprecated
    jsonGame["uuid"] = m_sUuid;
    jsonGame["name"] = m_sName;
    jsonGame["description"] = m_sDescription;
    jsonGame["state"] = m_sState;
    jsonGame["type"] = m_sType;
    jsonGame["form"] = m_sForm;
    jsonGame["date_start"] = m_sDateStart;
    jsonGame["date_stop"] = m_sDateStop;
    jsonGame["date_restart"] = m_sDateRestart;
    jsonGame["organizators"] = m_sOrganizators;
    jsonGame["maxscore"] = m_nMaxScore;
    return jsonGame;
}

// ---------------------------------------------------------------------

void ModelGame::fillFrom(const nlohmann::json &jsonGame) {
    // TODO local_id ?
    // TODO maxScore ?

    // uuid
    try {
        setUuid(jsonGame.at("uuid").get<std::string>()); // TODO trim
    } catch ( std::exception const&) {
        // nothing
    }

    // name, optional
    try {
        setName(jsonGame.at("name").get<std::string>()); // TODO trim
    } catch ( std::exception const&) {
        // nothing
    }

    // description, optional
    try {
        setDescription(jsonGame.at("description").get<std::string>()); // TODO trim
    } catch ( std::exception const& e ) {
        // nothing
    }

    // state, optional
    try {
        setState(jsonGame.at("state").get<std::string>()); // TODO trim
    } catch ( std::exception const&) {
        // nothing
    }

    // form, optional
    try {
        setForm(jsonGame.at("form").get<std::string>()); // TODO trim
    } catch ( std::exception const&) {
        // nothing
    }

    // type, optional
    try {
        setType(jsonGame.at("type").get<std::string>()); // TODO trim
    } catch ( std::exception const&) {
        // nothing
    }

    // date_start, optional
    try {
        setDateStart(jsonGame.at("date_start").get<std::string>()); // TODO trim
    } catch ( std::exception const&) {
        // nothing
    }

    // date_stop, optional
    try {
        setDateStop(jsonGame.at("date_stop").get<std::string>()); // TODO trim
    } catch ( std::exception const&) {
        // nothing
    }

    // date_restart, optional
    try {
        setDateRestart(jsonGame.at("date_restart").get<std::string>()); // TODO trim
    } catch ( std::exception const&) {
        // nothing
    }

    // organizators, optional
    try {
        setOrganizators(jsonGame.at("organizators").get<std::string>()); // TODO trim
    } catch ( std::exception const& ) {
        // nothing
    }
}

// ---------------------------------------------------------------------


