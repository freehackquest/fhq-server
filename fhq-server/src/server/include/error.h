#ifndef ERROR_H
#define ERROR_H

#include <string>

class Error {
	public:
        Error(int nCodeError, const std::string &sMessage);
		int codeError();
        std::string message();
	private:
        std::string m_sMessage;
		int m_nCodeError;
};

#endif // ERROR_H
