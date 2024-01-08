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


#include "update_csww6lwcrp_722kt134lq.h"

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_csww6lwcrp_722kt134lq)

Update_csww6lwcrp_722kt134lq::Update_csww6lwcrp_722kt134lq()
    : WsjcppStorageUpdateBase("csww6lwcrp", "722kt134lq", "useful_links - tags, favorites, comments") {

  // useful_links_user_favorites
  WsjcppStorageCreateTable *useful_links_user_favorites = createTable("useful_links_user_favorites");
  useful_links_user_favorites->addColumn("id").number().autoIncrement().primaryKey().notNull();
  useful_links_user_favorites->addColumn("usefullinkid").number().notNull().defaultValue("0");
  useful_links_user_favorites->addColumn("userid").number().notNull().defaultValue("0");
  useful_links_user_favorites->addColumn("dt").datetime().notNull();

  // useful_links_comments
  WsjcppStorageCreateTable *useful_links_comments = createTable("useful_links_comments");
  useful_links_comments->addColumn("id").number().autoIncrement().primaryKey().notNull();
  useful_links_comments->addColumn("usefullinkid").number().notNull().defaultValue("0");
  useful_links_comments->addColumn("userid").number().notNull().defaultValue("0");
  useful_links_comments->addColumn("comment").string(1024).notNull().defaultValue("");
  useful_links_comments->addColumn("dt").datetime().notNull();

  // useful_links_comments
  WsjcppStorageCreateTable *useful_links_tags = createTable("useful_links_tags");
  useful_links_tags->addColumn("id").number().autoIncrement().primaryKey().notNull();
  useful_links_tags->addColumn("usefullinkid").number().notNull().defaultValue("0");
  useful_links_tags->addColumn("tagvalue").string(32).notNull().defaultValue("");
  useful_links_tags->addColumn("counter").number().notNull().defaultValue("0");

  dropTable("useful_links_stars");

  WsjcppStorageModifyTable *useful_links = modifyTable("useful_links");
  useful_links->addColumn("user_favorites").number().notNull().defaultValue("0");
  useful_links->addColumn("user_clicks").number().notNull().defaultValue("0");
  useful_links->addColumn("user_comments").number().notNull().defaultValue("0");
  useful_links_tags->addColumn("tags").string(1024).notNull().defaultValue("");
}