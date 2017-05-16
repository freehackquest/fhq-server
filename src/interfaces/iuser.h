#ifndef INTERFACES_IUSER_H
#define INTERFACES_IUSER_H

class IUser {
	public:
		virtual bool isAdmin() = 0;
		virtual bool isUser() = 0;
		virtual bool isTester() = 0;
		virtual bool hasRole() = 0;
		virtual QString nick() = 0;
		virtual QString email() = 0;
		virtual int userid() = 0;
};

#endif // INTERFACES_IUSER_H
