#ifndef ERRORS_H
#define ERRORS_H

#include <QString>
#include "error.h"

class Errors {
	public:
		static void WriteServerError(QString errorInfo);
		static Error NotAuthorizedRequest();
		static Error AllowedOnlyForAdmin();
		static Error NotImplementedYet();
		static Error NotFoundUserByUUID(QString uuid);
		static Error NotFoundUUIDField();
		static Error LostDatabaseConnection();
		static Error QuestIDMustBeInteger();
		static Error QuestIDMustBeNotZero();
		static Error HintIDMustBeInteger();
		static Error HintIDMustBeNotZero();
		static Error UserIDMustBeInteger();
		static Error UserIDMustBeNotZero();
		static Error ParamTableMustBeActiveOrBackup();
		static Error OnPageCouldNotBeMoreThen50();
		static Error GameIDMustBeInteger();
		static Error OnPageMustBeInteger();
		static Error PageMustBeInteger();
		static Error InternalServerError();
		static Error EventIdMustBeInteger();
		static Error EventIdExpected();
		static Error EventNotFound();
		static Error EventTypeExpected();
		static Error EventMessageExpected();
		static Error EventTypeValueExpected();

	private:
		
};

#endif // ERRORS_H
