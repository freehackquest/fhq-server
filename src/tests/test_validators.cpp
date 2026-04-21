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

#include <iostream>
#include <validators/validators.h>
#include <vector>

int main() {
  struct LTestVld {
    LTestVld(WsjcppValidatorStringBase *pValidator, std::string sValue, bool bExpectedResult) {
      m_pValidator = pValidator;
      m_sValue = sValue;
      m_bExpectedResult = bExpectedResult;
    }

    WsjcppValidatorStringBase *m_pValidator;
    std::string m_sValue;
    int m_bExpectedResult;
  };

  std::vector<LTestVld *> tests;

  ValidatorGameState *pValidatorGameState = new ValidatorGameState();
  tests.push_back(new LTestVld(pValidatorGameState, "test", false));
  tests.push_back(new LTestVld(pValidatorGameState, "original", true));
  tests.push_back(new LTestVld(pValidatorGameState, "copy", true));
  tests.push_back(new LTestVld(pValidatorGameState, "unlicensed_copy", true));
  tests.push_back(new LTestVld(pValidatorGameState, "some", false));

  for (unsigned int i = 0; i < tests.size(); i++) {
    std::string sValue = tests[i]->m_sValue;
    WsjcppValidatorStringBase *pValidator = tests[i]->m_pValidator;
    bool bExpectedResult = tests[i]->m_bExpectedResult;
    std::string sError = "";
    bool bGotResult = pValidator->isValid(sValue, sError);
    if (bGotResult != bExpectedResult) {
      std::cerr << "Test '" << sValue << "' error: " << sError << std::endl;
      return -1;
    }
  }
  return 0;
}