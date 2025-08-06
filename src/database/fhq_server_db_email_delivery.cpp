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

#include "fhq_server_db_email_delivery.h"

#include <employees.h>

// ---------------------------------------------------------------------
// FhqServerDbEmailDelivery

FhqServerDbEmailDelivery::FhqServerDbEmailDelivery()
  : FhqServerDatabaseFile("email_delivery.db"
                          // "CREATE TABLE IF NOT EXISTS email_delivery ( "
                          // "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          // // "  uuid VARCHAR(36) NOT NULL,"
                          // "  url VARCHAR(2048) NOT NULL,"
                          // "  description VARCHAR(2048) NOT NULL,"
                          // "  author VARCHAR(127) NOT NULL,"
                          // "  dt INTEGER NOT NULL,"
                          // "  user_favorites INTEGER NOT NULL,"
                          // "  user_clicks INTEGER NOT NULL,"
                          // "  user_comments INTEGER NOT NULL,"
                          // "  tags VARCHAR(2048) NOT NULL"
                          // ");"
    ) {
  TAG = "FhqServerDbEmailDelivery";
};

FhqServerDbEmailDelivery::~FhqServerDbEmailDelivery() {}
