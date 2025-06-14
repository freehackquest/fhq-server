/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2025 FreeHackQuest <freehackquest@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#include "employ_mails.h"
#include <employ_database.h>
#include <employ_notify.h>

REGISTRY_WJSCPP_EMPLOY(EmployMails)

// ---------------------------------------------------------------------

EmployMails::EmployMails()
  : WsjcppEmployBase(
      EmployMails::name(), {EmployGlobalSettings::name(), EmployDatabase::name(), EmployNotify::name()}
    ) {
  TAG = EmployMails::name();

  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

  std::string sGroupMail = "mail";
  pGlobalSettings->registrySetting(sGroupMail, "mail_from").string("freehackquest@gmail.com").inDatabase();
  pGlobalSettings->registrySetting(sGroupMail, "mail_host").string("smtp.gmail.com").inDatabase();
  pGlobalSettings->registrySetting(sGroupMail, "mail_username").string("freehackquest@gmail.com").inDatabase();
  pGlobalSettings->registrySetting(sGroupMail, "mail_port").number(465).inDatabase();
  pGlobalSettings->registrySetting(sGroupMail, "mail_password").password("some").inDatabase();
  pGlobalSettings->registrySetting(sGroupMail, "mail_auth").boolean(true).inDatabase();
  pGlobalSettings->registrySetting(sGroupMail, "mail_allow").boolean(true).inDatabase();
  pGlobalSettings->registrySetting(sGroupMail, "mail_system_message_admin_email").string("").inDatabase();
  pGlobalSettings->registrySetting(sGroupMail, "mail_signature_text")
    .text("---\r\n"
          "With thanks,\r\n"
          "FreeHackQuest Team\r\n"
          "\r\n"
          "Write message via e-mail to freehackquest@gmail.com\r\n"
          "Contact by Telegram: https://t.me/freehackquest\r\n")
    .inDatabase();
}

// ---------------------------------------------------------------------

bool EmployMails::init() {
  // TODO
  return true;
}

// ---------------------------------------------------------------------

bool EmployMails::deinit() {
  // TODO
  return true;
}

// ---------------------------------------------------------------------
