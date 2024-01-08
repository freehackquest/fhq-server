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

#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <string>
#include <vector>
#include <wsjcpp_validators.h>

// ---------------------------------------------------------------------

class ValidatorEventType : public WsjcppValidatorStringBase {
public:
  ValidatorEventType();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
  std::vector<std::string> m_vEventTypes;
};

// ---------------------------------------------------------------------

class ValidatorQuestSubject : public WsjcppValidatorStringBase {
public:
  ValidatorQuestSubject();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
  std::vector<std::string> m_vQuestSubjects;
};

// ---------------------------------------------------------------------

class ValidatorQuestState : public WsjcppValidatorStringBase {
public:
  ValidatorQuestState();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
  std::vector<std::string> m_vQuestStates;
};

// ---------------------------------------------------------------------

class ValidatorGameForm : public WsjcppValidatorStringBase {
public:
  ValidatorGameForm();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
  std::vector<std::string> m_vGameForms;
};

// ---------------------------------------------------------------------

class ValidatorGameState : public WsjcppValidatorStringBase {
public:
  ValidatorGameState();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
  std::vector<std::string> m_vGameStates;
};

// ---------------------------------------------------------------------

class ValidatorGameType : public WsjcppValidatorStringBase {
public:
  ValidatorGameType();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
  std::vector<std::string> m_vGameTypes;
};

// ---------------------------------------------------------------------

class ValidatorLXDFileActionType : public WsjcppValidatorStringBase {
public:
  ValidatorLXDFileActionType();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
  std::vector<std::string> m_vActionTypes;
};

// ----------------------------------------------------------------------

class ValidatorLanguage : public WsjcppValidatorStringListBase {
public:
  ValidatorLanguage();
};

// ----------------------------------------------------------------------

#endif // VALIDATORS_H
