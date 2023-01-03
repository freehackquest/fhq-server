/*
MIT License

Copyright (c) 2011-2023 FreeHackQuest <freehackquest@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <update0101.h>

REGISTRY_WSJCPP_STORAGE_UPDATE(Update0101)

Update0101::Update0101() : WsjcppStorageUpdateBase("u0100", "u0101", "Add new table 'users_requests'") {

  {
    WsjcppStorageCreateTable *pUserRequests = createTable("users_requests");
    pUserRequests->addColumn("id").number().autoIncrement().primaryKey().notNull();
    pUserRequests->addColumn("email").string(255).notNull();
    pUserRequests->addColumn("type").string(255).notNull().defaultValue("''");
    pUserRequests->addColumn("code").string(32).notNull();
    pUserRequests->addColumn("dt").datetime().notNull();
    pUserRequests->addColumn("status").string(10).notNull().defaultValue("'prepared'");
    pUserRequests->addColumn("data").string(2048).notNull().enableIndex();
  }
}
