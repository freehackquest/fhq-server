#include "employ_mails.h"
#include <employ_database.h>
#include <employ_notify.h>

REGISTRY_WJSCPP_EMPLOY(EmployMails)

// ---------------------------------------------------------------------

EmployMails::EmployMails()
    : WSJCppEmployBase(EmployMails::name(), { EmployGlobalSettings::name(), EmployDatabase::name(), EmployNotify::name() }) {
    TAG = EmployMails::name();

    EmployGlobalSettings *pGlobalSettings = findEmploy<EmployGlobalSettings>();
    
    std::string  sGroupMail = "mail";
    pGlobalSettings->registrySetting(sGroupMail, "mail_from").string("freehackquest@gmail.com").inDatabase();
    pGlobalSettings->registrySetting(sGroupMail, "mail_host").string("smtp.gmail.com").inDatabase();
    pGlobalSettings->registrySetting(sGroupMail, "mail_username").string("freehackquest@gmail.com").inDatabase();
    pGlobalSettings->registrySetting(sGroupMail, "mail_port").number(465).inDatabase();
    pGlobalSettings->registrySetting(sGroupMail, "mail_password").password("some").inDatabase();
    pGlobalSettings->registrySetting(sGroupMail, "mail_auth").boolean(true).inDatabase();
    pGlobalSettings->registrySetting(sGroupMail, "mail_allow").boolean(true).inDatabase();
    pGlobalSettings->registrySetting(sGroupMail, "mail_system_message_admin_email").string("").inDatabase();
    pGlobalSettings->registrySetting(sGroupMail, "mail_signature_text").text(
        "---\r\n"
        "With thanks,\r\n"
        "FreeHackQuest Team\r\n"
        "\r\n"
        "Write message via e-mail to freehackquest@gmail.com\r\n"
        "Contact by Telegram: https://t.me/freehackquest\r\n"
        "Follow in Twitter: https://twitter.com/freehackquest\r\n"
        "Google+ Community: https://plus.google.com/communities/107112500285357136727\r\n"
    ).inDatabase();
}

// ---------------------------------------------------------------------

bool EmployMails::init() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------
