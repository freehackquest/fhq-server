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

#pragma once

#include <wsjcpp_arguments.h>

class ArgumentProcessorConfig : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfig();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigTest : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigTest();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigPrint : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigPrint();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigTestEmail : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigTestEmail();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigSet : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigSet();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigLxdEnable : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigLxdEnable();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigLxdDisable : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigLxdDisable();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorConfigLxdConfigure : public WsjcppArgumentProcessor {
public:
  ArgumentProcessorConfigLxdConfigure();
  virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};
