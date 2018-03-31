#ifndef MODEL_COMMAND_ACCESS_H
#define MODEL_COMMAND_ACCESS_H

#include <iwebsocketserver.h>

class ModelCommandAccess {
    public:
        ModelCommandAccess(bool bAccessUnauthorized, bool bAccessUser, bool bAccessAdmin);
        ModelCommandAccess();

        bool accessUnauthorized();
        bool accessUnauthorized() const;
        bool accessUser();
        bool accessUser() const;
        bool accessAdmin();
        bool accessAdmin() const;

        void setAccessUnauthorized(bool bNewValue);
        void setAccessUser(bool bNewValue);
        void setAccessAdmin(bool bNewValue);

	private:
        bool m_bAccessUnauthorized;
        bool m_bAccessUser;
        bool m_bAccessAdmin;
};

#endif // MODEL_COMMAND_ACCESS_H
