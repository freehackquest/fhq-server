/*
MIT License

Copyright (c) 2020-2025 wsjcpp

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

#pragma once

#include <regex>
#include <string>
#include <vector>

enum WsjcppValidatorType {
  WSJCPP_VALIDATOR_STRING,
  WSJCPP_VALIDATOR_INTEGER,
  WSJCPP_VALIDATOR_JSON
};

class WsjcppValidators {
public:
  static bool isValidDate(const std::string &sValue, std::string &sError);
  static bool isValidTimeH24(const std::string &sValue, std::string &sError);
  static bool isValidURLProtocol(const std::string &sValue,
                                 std::string &sError);
  static bool isValidDomainName(const std::string &sValue, std::string &sError);
  static bool isValidPort(const std::string &sValue, std::string &sError);
  static bool isValidPort(int nValue, std::string &sError);
  static bool isValidBase64(const std::string &sValue, std::string &sError);
  static bool isValidIPv4(const std::string &sValue, std::string &sError);
  static bool isValidIPv6(const std::string &sValue, std::string &sError);
};

/*
// ----------------------------------------------------------------------

class WsjcppValidatorBase {
    public:
        WsjcppValidatorBase(const std::string &sTypeName, WsjcppValidatorType
nValidatorType); virtual WsjcppValidatorType getBaseType(); virtual std::string
getTypeName(); virtual bool isValid(const std::string &sValue, std::string
&sError) = 0; protected: std::string TAG; private: std::string m_sTypeName;
};

*/

class WsjcppValidatorStringBase {
public:
  WsjcppValidatorStringBase(const std::string &typeName);
  virtual WsjcppValidatorType getBaseType();
  virtual std::string getTypeName();
  virtual bool isValid(const std::string &sValue, std::string &sError) = 0;

protected:
  std::string TAG;

private:
  std::string m_sTypeName;
};

class WsjcppValidatorStringRegexpBase : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorStringRegexpBase(const std::string &typeName,
                                  const std::string &sValidator);
  virtual bool isValid(const std::string &sValue, std::string &sError) override;

private:
  std::string m_sValidator;
  std::regex m_rxValidator;
};

class WsjcppValidatorStringListBase : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorStringListBase(const std::string &typeName,
                                const std::vector<std::string> &vListValues);
  virtual bool isValid(const std::string &sValue, std::string &sError) override;

private:
  std::vector<std::string> m_vListValues;
};

class WsjcppValidatorEmail : public WsjcppValidatorStringRegexpBase {
public:
  WsjcppValidatorEmail();
};

class WsjcppValidatorUUID : public WsjcppValidatorStringRegexpBase {
public:
  WsjcppValidatorUUID();
};

class WsjcppValidatorStringLength : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorStringLength(int nMinLength, int nMaxLength);
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
  int m_nMinLength;
  int m_nMaxLength;
};

class WsjcppValidatorJWT : public WsjcppValidatorStringRegexpBase {
public:
  WsjcppValidatorJWT();
};

class WsjcppValidatorDate : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorDate();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
};

class WsjcppValidatorTimeH24 : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorTimeH24();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
};

class WsjcppValidatorDateTime : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorDateTime();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
};

class WsjcppValidatorURL : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorURL();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
  std::regex m_rxLikeIPv4Format;
};

class WsjcppValidatorDomainName : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorDomainName();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
};

class WsjcppValidatorBase64 : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorBase64();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
};

class WsjcppValidatorNumber : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorNumber();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
};

class WsjcppValidatorHex : public WsjcppValidatorStringBase {
public:
  WsjcppValidatorHex();
  virtual bool isValid(const std::string &sValue, std::string &sError);

private:
  std::string TAG;
};

class WsjcppValidatorIntegerBase {
public:
  WsjcppValidatorIntegerBase(const std::string &typeName);
  virtual WsjcppValidatorType getBaseType();
  virtual std::string getTypeName();
  virtual bool isValid(int nValue, std::string &sError) = 0;

protected:
  std::string TAG;

private:
  std::string m_sTypeName;
};

class WsjcppValidatorIntegerMinValue : public WsjcppValidatorIntegerBase {
public:
  WsjcppValidatorIntegerMinValue(int nMinValue);
  virtual bool isValid(int nValue, std::string &sError) override;

private:
  std::string TAG;
  int m_nMinValue;
};

class WsjcppValidatorIntegerMaxValue : public WsjcppValidatorIntegerBase {
public:
  WsjcppValidatorIntegerMaxValue(int nMaxValue);
  virtual bool isValid(int nValue, std::string &sError) override;

private:
  std::string TAG;
  int m_nMaxValue;
};
