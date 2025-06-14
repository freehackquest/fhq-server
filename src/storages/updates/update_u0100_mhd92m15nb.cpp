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

#include "update_u0100_mhd92m15nb.h"

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_u0100_mhd92m15nb)

Update_u0100_mhd92m15nb::Update_u0100_mhd92m15nb()
  : WsjcppStorageUpdateBase("u0100", "mhd92m15nb", "Creating table useful_links") {

  WsjcppStorageCreateTable *useful_links = createTable("useful_links");
  useful_links->addColumn("id").number().autoIncrement().primaryKey().notNull();
  useful_links->addColumn("url").string(2048).notNull().defaultValue("");
  useful_links->addColumn("description").string(2048).notNull().defaultValue("");
  useful_links->addColumn("author").string(127).notNull().defaultValue("");
  useful_links->addColumn("stars").number().notNull().defaultValue("0");
  useful_links->addColumn("dt").datetime().notNull();

  WsjcppStorageCreateTable *useful_links_stars = createTable("useful_links_stars");
  useful_links_stars->addColumn("id").number().autoIncrement().primaryKey().notNull();
  useful_links_stars->addColumn("usefullinkid").number().notNull().defaultValue("0");
  useful_links_stars->addColumn("userid").number().notNull().defaultValue("0");
  useful_links_stars->addColumn("dt").datetime().notNull();
}