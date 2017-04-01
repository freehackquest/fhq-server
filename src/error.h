#ifndef ERROR_H
#define ERROR_H

#include <QString>

class Error {
	public:
		Error(int codeError, QString message);
		int codeError();
		QString message();
	private:
		QString m_sMessage;
		int m_nCodeError;
};

#endif // ERROR_H
