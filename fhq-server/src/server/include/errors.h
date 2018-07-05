#ifndef ERRORS_H
#define ERRORS_H

#include <QString>
#include "error.h"

class Errors {
	public:
		
		static Error NotAuthorizedRequest();
		static Error AccessDenyForUser();
		static Error AccessDenyForTester();
		static Error AccessDenyForAdmin();
		static Error InvalidToken();
		static Error LostDatabaseConnection();

		static Error NotImplementedYet();
        static Error NotImplementedYet(const QString &comments);
		static Error InternalServerError();
		static Error ParamExpected(QString param_name);
		static Error ParamMustBeInteger(QString param_name);
        static Error ParamMustBeEmail(QString param_name);
		static Error ParamExpectedValueOneFrom(QString param_name, QStringList eList);
		static Error ParamExpectedUUID(QString param_name);
		static Error ParamMustBeLessThen(QString param_name, int maxval);
		static Error ParamMustBeMoreThen(QString param_name, int minval);
		static Error NoneError();
		static Error NotFound(QString what);
        static Error DatabaseError(QString text);
		
		// custom
		static Error QuestIDMustBeNotZero();
		static Error HintIDMustBeNotZero();
		static Error UserIDMustBeNotZero();
		static Error ParamTableMustBeActiveOrBackup();
		static Error OnPageCouldNotBeMoreThen50();
        static Error EventNotFound();
		
	private:
		
};

#endif // ERRORS_H
