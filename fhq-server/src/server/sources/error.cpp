#include <error.h>

// ---------------------------------------------------------------------

Error::Error(int nCodeError, const std::string &sMessage){
    m_nCodeError = nCodeError;
    m_sMessage = sMessage;
}

// ---------------------------------------------------------------------

int Error::codeError(){
	return m_nCodeError;
}

// ---------------------------------------------------------------------

std::string Error::message(){
	return m_sMessage;
}
