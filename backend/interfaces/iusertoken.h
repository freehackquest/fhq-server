#ifndef INTERFACES_IUSERTOKEN_H
#define INTERFACES_IUSERTOKEN_H

class IUserToken {
    public:
        virtual void fillFromJson(QJsonObject obj) = 0;
        virtual bool isAdmin() = 0;
        virtual bool isUser() = 0;
        virtual bool isTester() = 0;
        virtual bool hasRole() = 0;
        virtual QString nick() = 0;
        virtual void setNick(QString) = 0;
        virtual QString email() = 0;
        virtual int userid() = 0;
};

#endif // INTERFACES_IUSERTOKEN_H
