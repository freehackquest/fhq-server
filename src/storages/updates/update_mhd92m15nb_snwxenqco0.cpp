/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
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


#include "update_mhd92m15nb_snwxenqco0.h"

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_mhd92m15nb_snwxenqco0)

Update_mhd92m15nb_snwxenqco0::Update_mhd92m15nb_snwxenqco0()
    : WsjcppStorageUpdateBase("mhd92m15nb", "snwxenqco0", "Added tables chats, chats_users, chats_messages") {

  WsjcppStorageCreateTable *chat = createTable("chats");
  chat->addColumn("id").number().autoIncrement().primaryKey().notNull();
  chat->addColumn("uuid").string(127).notNull().defaultValue("");
  chat->addColumn("name").string(127).notNull().defaultValue("");
  chat->addColumn("type").string(127).notNull().defaultValue("");
  chat->addColumn("owner").number().notNull().defaultValue("0");
  chat->addColumn("last_message_id").datetime().notNull();
  chat->addColumn("created").datetime().notNull();
  chat->addColumn("updated").datetime().notNull();

  WsjcppStorageCreateTable *chats_users = createTable("chats_users");
  chats_users->addColumn("id").number().autoIncrement().primaryKey().notNull();
  chats_users->addColumn("chatid").number().notNull().defaultValue("0");
  chats_users->addColumn("userid").number().notNull().defaultValue("0");
  chats_users->addColumn("chat_role").string(127).notNull().defaultValue(""); // TODO enums: invited/blocked/admin/user
  chats_users->addColumn("visited").datetime().notNull();

  WsjcppStorageCreateTable *chats_messages = createTable("chats_messages");
  chats_messages->addColumn("id").number().autoIncrement().primaryKey().notNull();
  chats_messages->addColumn("chatid").number().notNull().defaultValue("0");
  chats_messages->addColumn("userid").number().notNull().defaultValue("0");
  chats_messages->addColumn("message").string(1024).notNull().defaultValue("");
  chats_messages->addColumn("status").string(127).notNull().defaultValue("");
  chats_messages->addColumn("dt").string(127).notNull().defaultValue("");
  // chats_messages->addColumn("created").datetime().notNull();
  // chats_messages->addColumn("updated").datetime().notNull();
}
