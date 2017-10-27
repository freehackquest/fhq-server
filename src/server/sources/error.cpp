#include <error.h>

Error::Error(int codeError, QString message){
	m_nCodeError = codeError;
	m_sMessage = message;
}

int Error::codeError(){
	return m_nCodeError;
}

QString Error::message(){
	return m_sMessage;
}
