/**********************************************************************************
 *                      Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * MIT License
 *
 * Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
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
 * Original repository: https://github.com/freehackquest/fhq-server
 *
 ***********************************************************************************/

#include <vector>
#include <iostream>

#include <wsjcpp_storages.h>

int main() {
  if (!WsjcppStorages::support("mysql")) {
    std::cerr << "Not supported mysql" << std::endl;
    return -1;
  }

  WsjcppStorage *pStorage = WsjcppStorages::create("mysql");
  struct LTest {
    LTest(std::string sql_in, std::string sql_expected) : sql_in(sql_in), sql_expected(sql_expected) {}
    std::string sql_in;
    std::string sql_expected;
  };


  std::vector<LTest *> tests;
  tests.push_back(new LTest("\'\"", "\\\'\\\""));
  tests.push_back(new LTest("test usually string 12 34", "test usually string 12 34"));
  tests.push_back(new LTest("test1 \r\n text2", "test1 \\r\\n text2"));
  tests.push_back(new LTest("test1 \0 text2", "test1 "));

  unsigned int nSuccess = 0;
  for (unsigned int i = 0; i < tests.size(); i++) {
    std::string sql_in = tests[i]->sql_in;
    std::string sql_expected = "\"" + tests[i]->sql_expected + "\"";
    std::string sql_got = pStorage->prepareStringValue(sql_in);
    if (sql_expected != sql_got) {
      std::cerr << "Expected '" << sql_expected << "', but got '" << sql_got << "'" << std::endl;
      return -1;
    }
  }
  return 0;
}
