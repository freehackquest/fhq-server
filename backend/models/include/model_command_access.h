#ifndef MODEL_COMMAND_ACCESS_H
#define MODEL_COMMAND_ACCESS_H

#include <iwebsocketserver.h>

class ModelCommandAccess {
    public:
        ModelCommandAccess(bool bAccessUnauthorized, bool bAccessUser, bool bAccessTester, bool bAccessAdmin);

        bool accessUnauthorized();
        bool accessUser();
        bool accessTester();
        bool accessAdmin();

	private:
        bool m_bAccessUnauthorized;
        bool m_bAccessUser;
        bool m_bAccessTester;
        bool m_bAccessAdmin;
};

#endif // MODEL_COMMAND_ACCESS_H
