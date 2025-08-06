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

#include "wsjcpp_validators.h"
#include <arpa/inet.h>
#include <wsjcpp_core.h>

bool WsjcppValidators::isValidDate(const std::string &sValue,
                                   std::string &sError) {
  int nSize = sValue.size();
  if (nSize != 10) {
    sError = "Invalid size format expected length 10";
    return false;
  }

  for (int i = 0; i < 10; i++) {
    char c = sValue[i];
    if (i == 4 || i == 7) {
      if (c != '-') {
        sError =
            "Expected '-' in " + std::to_string(i) + " position, but got '";
        sError += c;
        sError += "'";
        return false;
      }
      continue;
    }
    if (c < '0' || c > '9') {
      sError = "Unexpected char '";
      sError += c;
      sError += "' in " + std::to_string(i) + " position";
      return false;
    }
  }
  // 2020-01-01
  std::string sYear = sValue.substr(0, 4);
  int nYear = std::atoi(sYear.c_str());

  std::string sMonth = sValue.substr(5, 2);
  int nMonth = std::atoi(sMonth.c_str());
  if (nMonth < 1 || nMonth > 12) {
    sError = "Invalid value nunber of month '" + std::to_string(nMonth) +
             "' expected 01..12";
    return false;
  }

  int nMaxDay = 0;
  if (nMonth == 1 || nMonth == 3 || nMonth == 5 || nMonth == 7 || nMonth == 8 ||
      nMonth == 10 || nMonth == 12) {
    nMaxDay = 31;
  } else if (nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11) {
    nMaxDay = 30;
  } else if (nMonth == 2 && nYear % 4 == 0) {
    nMaxDay = 29;
  } else if (nMonth == 2 && nYear % 4 != 0) {
    nMaxDay = 28;
  }

  std::string sDay = sValue.substr(8, 2);
  int nDay = std::atoi(sDay.c_str());
  if (nDay < 1 || nDay > nMaxDay) {
    sError = "Invalid value number of day '" + std::to_string(nDay) +
             "' expected 01.." + std::to_string(nMaxDay);
    return false;
  }
  return true;
}

bool WsjcppValidators::isValidTimeH24(const std::string &sValue,
                                      std::string &sError) {
  int nSize = sValue.size();
  if (nSize != 8) {
    sError = "Invalid size format expected length 8";
    return false;
  }

  for (int i = 0; i < 8; i++) {
    char c = sValue[i];
    if (i == 2 || i == 5) {
      if (c != ':') {
        sError =
            "Expected ':' in " + std::to_string(i) + " position, but got '";
        sError += c;
        sError += "'";
        return false;
      }
      continue;
    }
    if (c < '0' || c > '9') {
      sError = "Unexpected char '";
      sError += c;
      sError += "' in " + std::to_string(i) + " position";
      return false;
    }
  }

  std::string sHours = sValue.substr(0, 2);
  int nHours = std::atoi(sHours.c_str());
  if (nHours > 23) {
    sError = "Invalid value of hours '" + std::to_string(nHours) +
             "' expected 00..23";
    return false;
  }
  std::string sMinutes = sValue.substr(3, 2);
  int nMinutes = std::atoi(sMinutes.c_str());
  if (nMinutes > 59) {
    sError = "Invalid value of minutes '" + std::to_string(nMinutes) +
             "' expected 00..59";
    return false;
  }

  std::string sSeconds = sValue.substr(6, 2);
  int nSeconds = std::atoi(sSeconds.c_str());
  if (nSeconds > 59) {
    sError = "Invalid value of seconds '" + std::to_string(nSeconds) +
             "' expected 00..59";
    return false;
  }
  return true;
}

bool WsjcppValidators::isValidDomainName(const std::string &sValue,
                                         std::string &sError) {
  std::vector<std::string> vSubDomains;
  std::string sTmpDomain = "";
  int nAddressLen = sValue.size();
  char cPrev = 0;
  for (int i = 0; i < nAddressLen; i++) {
    char c = sValue[i];
    if (i == 0 && c == '.') {
      sError = "Domain Name '" + sValue + "' could not be start on '.'";
      return false;
    }
    if (c == '.') {
      if (sTmpDomain != "") {
        vSubDomains.push_back(sTmpDomain);
        sTmpDomain = "";
        continue;
      } else {
        sError = "Domain Name '" + sValue + "' could not contains '..'";
        return false;
      }
    }
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
        (c >= '0' && c <= '9') || c == '-') {
      sTmpDomain += c;
      if (cPrev == '-' && c == cPrev) {
        sError = "Domain Name '" + sValue + "' could not two times in a row '";
        sError += c;
        sError += c;
        sError += "'";
        return false;
      }
      cPrev = c;
    } else {
      sError = "Domain Name '" + sValue + "' contains unexpected '";
      sError += c;
      sError += "'";
      return false;
    }
  }

  if (sTmpDomain != "") {
    vSubDomains.push_back(sTmpDomain);
    sTmpDomain = "";
  } else {
    sError = "Domain Name '" + sValue + "' could not contains '.' on end";
    return false;
  }

  if (vSubDomains.size() < 2) {
    sError = "Domain Name '" + sValue + "' must contains least one dot";
    return false;
  }
  std::string sRootDomain = vSubDomains[vSubDomains.size() - 1];
  if (sRootDomain.size() < 2) {
    sError = "Domain Name '" + sValue + "' has wrong root domain '" +
             sRootDomain + "' length must be more then 1";
    return false;
  }
  for (int i = 0; i < sRootDomain.size(); i++) {
    char c = sRootDomain[i];
    if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z')) {
      sError = "Domain Name '" + sValue + "' has wrong root domain '" +
               sRootDomain + "' must have only chars";
      return false;
    }
  }

  for (int i = 0; i < vSubDomains.size(); i++) {
    std::string sDomain = vSubDomains[i];
    char c = sDomain[0];
    if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && (c < '0' || c > '9')) {
      sError = "Subdomain '" + sDomain + "' could not start on '";
      sError += c;
      sError += "'";
      return false;
    }
    c = sDomain[sDomain.size() - 1];
    if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && (c < '0' || c > '9')) {
      sError = "Subdomain '" + sDomain + "' could not end on '";
      sError += c;
      sError += "'";
      return false;
    }
  }
  return true;
}

bool WsjcppValidators::isValidPort(const std::string &sValue,
                                   std::string &sError) {
  int nPort = std::atoi(sValue.c_str());
  return WsjcppValidators::isValidPort(nPort, sError);
}

bool WsjcppValidators::isValidPort(int nValue, std::string &sError) {
  if (nValue < 1 || nValue > 65535) {
    sError = "Port '" + std::to_string(nValue) +
             "' must be more then 0 and less then 65536";
    return false;
  }
  return true;
}

bool WsjcppValidators::isValidURLProtocol(const std::string &sValue,
                                          std::string &sError) {
  if (sValue != "http" && sValue != "https" && sValue != "ws" &&
      sValue != "wss" && sValue != "ftp" && sValue != "ssl") {
    sError = "Unexpected protocol '" + sValue + "'";
    return false;
  }
  return true;
}

bool WsjcppValidators::isValidBase64(const std::string &sValue,
                                     std::string &sError) {
  int nSize = sValue.size();
  if (nSize % 4 != 0) {
    sError = "Value size must be a multiple of 4";
    return false;
  }
  bool bLastChar = false;
  for (int i = 0; i < nSize; i++) {
    char c = sValue[i];
    if (!bLastChar && c == '=') {
      bLastChar = true;
      continue;
    }
    if (bLastChar && c == '=') {
      continue;
    }

    if (bLastChar && c != '=') {
      sError = "Unexpected char '";
      sError += c;
      sError += "' after '=' in " + std::to_string(i) + " position";
      return false;
    }

    if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && (c < '0' || c > '9') &&
        c != '+' && c != '/') {
      sError = "Unexpected char '";
      sError += c;
      sError += "' in " + std::to_string(i) + " position";
      return false;
    }
  }
  return true;
}

bool WsjcppValidators::isValidIPv4(const std::string &sValue,
                                   std::string &sError) {
  int n = 0;
  std::string s[4] = {"", "", "", ""};
  for (int i = 0; i < sValue.length(); i++) {
    char c = sValue[i];
    if (n > 3) {
      sError = "Groups number must be less than 5 (like '0.0.0.0')";
      return false;
    }
    if (c >= '0' && c <= '9') {
      s[n] += c;
    } else if (c == '.') {
      n++;
    } else {
      sError = "Unexpected character '";
      sError += c;
      sError += "'";
      return false;
    }
  }
  for (int i = 0; i < 4; i++) {
    if (s[i].length() > 3) {
      sError =
          "Value '" + s[i] + "' could not contains more than 3 digits in a row";
      return false;
    }
    int p = std::stoi(s[i]);
    if (p > 255 || p < 0) {
      sError = "Value '" + std::to_string(p) + "' must be 0..255";
      return false;
    }
  }
  return true;
}

bool WsjcppValidators::isValidIPv6(const std::string &sValue,
                                   std::string &sError) {
  // TODO redesign without arpa
  unsigned char buf[sizeof(struct in6_addr)];
  bool isValid = inet_pton(AF_INET6, sValue.c_str(), buf);
  if (!isValid) {
    sError = "Format of ipv6 invalid";
  }
  return isValid;
}

// ----------------------------------------------------------------------
// WsjcppValidatorStringBase

WsjcppValidatorStringBase::WsjcppValidatorStringBase(
    const std::string &sTypeName) {
  TAG = "WsjcppValidatorStringBase";
  m_sTypeName = sTypeName;
}

WsjcppValidatorType WsjcppValidatorStringBase::getBaseType() {
  return WsjcppValidatorType::WSJCPP_VALIDATOR_STRING;
}

std::string WsjcppValidatorStringBase::getTypeName() { return m_sTypeName; }

// ----------------------------------------------------------------------
// WsjcppValidatorStringRegexpBase

WsjcppValidatorStringRegexpBase::WsjcppValidatorStringRegexpBase(
    const std::string &typeName, const std::string &sValidator)
    : WsjcppValidatorStringBase(typeName) {
  TAG = "ValidatorStringRegexpBase";
  m_sValidator = sValidator;
  m_rxValidator = std::regex(sValidator);
}

bool WsjcppValidatorStringRegexpBase::isValid(const std::string &sValue,
                                              std::string &sError) {
  if (!std::regex_match(sValue, m_rxValidator)) {
    sError = getTypeName() + " - Value must match regular expression " +
             m_sValidator;
    return false;
  }
  return true;
}

// ----------------------------------------------------------------------
// WsjcppValidatorStringListBase

WsjcppValidatorStringListBase::WsjcppValidatorStringListBase(
    const std::string &sTypeName, const std::vector<std::string> &vListValues)
    : WsjcppValidatorStringBase(sTypeName) {
  m_vListValues = vListValues;
}

bool WsjcppValidatorStringListBase::isValid(const std::string &sValue,
                                            std::string &sError) {
  if (std::find(m_vListValues.begin(), m_vListValues.end(), sValue) !=
      m_vListValues.end()) {
    return true;
  }
  sError = getTypeName() + " expected one of [";
  for (int i = 0; i < m_vListValues.size(); i++) {
    sError += "'" + m_vListValues[i] + "'";
    if (i < m_vListValues.size() - 1) {
      sError += ", ";
    }
  }
  sError += "]";
  return false;
}

// ----------------------------------------------------------------------
// WsjcppValidatorEmail

WsjcppValidatorEmail::WsjcppValidatorEmail()
    : WsjcppValidatorStringRegexpBase(
          "email", "^[0-9a-zA-Z]{1}[0-9a-zA-Z-._]*[0-9a-zA-Z]{1}@[0-9a-zA-Z]{1}"
                   "[-.0-9a-zA-Z]*[0-9a-zA-Z]{1}\\.[a-zA-Z]{2,6}$") {
  TAG = "WsjcppValidatorEmail";
}

// ----------------------------------------------------------------------
// WsjcppValidatorUUID

WsjcppValidatorUUID::WsjcppValidatorUUID()
    : WsjcppValidatorStringRegexpBase("uuid",
                                      "^[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-"
                                      "f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}$") {
  TAG = "WsjcppValidatorUUID";
}

// ----------------------------------------------------------------------
// WsjcppValidatorStringLenght

WsjcppValidatorStringLength::WsjcppValidatorStringLength(int nMinLength,
                                                         int nMaxLength)
    : WsjcppValidatorStringBase("string_length") {
  TAG = "WsjcppValidatorStringLenght";
  m_nMinLength = nMinLength;
  m_nMaxLength = nMaxLength;
}

bool WsjcppValidatorStringLength::isValid(const std::string &sValue,
                                          std::string &sError) {
  if (sValue.length() < m_nMinLength) {
    sError = "Value must have more than " + std::to_string(m_nMinLength) +
             " simbols";
    return false;
  }

  if (sValue.length() > m_nMaxLength) {
    sError = "Value must have less than " + std::to_string(m_nMaxLength) +
             " simbols";
    return false;
  }
  return true;
}

// ----------------------------------------------------------------------
// WsjcppValidatorJWT

WsjcppValidatorJWT::WsjcppValidatorJWT()
    : WsjcppValidatorStringRegexpBase(
          "jwt", "^[A-Za-z0-9-_=]+\\.[A-Za-z0-9-_=]+\\.?[A-Za-z0-9-_.+/=]*$") {
  TAG = "WsjcppValidatorJWT";
}

// ----------------------------------------------------------------------
// WsjcppValidatorDate

WsjcppValidatorDate::WsjcppValidatorDate() : WsjcppValidatorStringBase("date") {
  TAG = "WsjcppValidatorDate";
}

// ----------------------------------------------------------------------

bool WsjcppValidatorDate::isValid(const std::string &sValue,
                                  std::string &sError) {
  return WsjcppValidators::isValidDate(sValue, sError);
}

// ----------------------------------------------------------------------
// WsjcppValidatorTimeH24

WsjcppValidatorTimeH24::WsjcppValidatorTimeH24()
    : WsjcppValidatorStringBase("time_h24") {
  TAG = "WsjcppValidatorTime";
}

bool WsjcppValidatorTimeH24::isValid(const std::string &sValue,
                                     std::string &sError) {
  return WsjcppValidators::isValidTimeH24(sValue, sError);
}

// ----------------------------------------------------------------------
// WsjcppValidatorDateTime

WsjcppValidatorDateTime::WsjcppValidatorDateTime()
    : WsjcppValidatorStringBase("datetime") {
  TAG = "WsjcppValidatorDateTime";
}

bool WsjcppValidatorDateTime::isValid(const std::string &sValue,
                                      std::string &sError) {
  int nSize = sValue.size();
  // '2020-01-01T00:00:00'
  if (nSize != 19) {
    sError = "Invalid size format expected length 19";
    return false;
  }
  std::string sDate = sValue.substr(0, 10);
  if (!WsjcppValidators::isValidDate(sDate, sError)) {
    return false;
  }
  if (sValue[10] != 'T') {
    sError = "Expected 'T' in 10 position, but got '";
    sError += sValue[10];
    sError += "'";
    return false;
  }
  std::string sTime = sValue.substr(11, 8);
  if (!WsjcppValidators::isValidTimeH24(sTime, sError)) {
    return false;
  }
  return true;
}

// ----------------------------------------------------------------------
// WsjcppValidatorURL

WsjcppValidatorURL::WsjcppValidatorURL() : WsjcppValidatorStringBase("url") {
  TAG = "WsjcppValidatorURL";
  m_rxLikeIPv4Format =
      std::regex("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
}

bool WsjcppValidatorURL::isValid(const std::string &sValue,
                                 std::string &sError) {
  if (sValue.size() == 0) {
    sError = "Value is empty";
    return false;
  }
  if (sValue.find(".") == std::string::npos) {
    sError = "Must contain at least one dot";
    return false;
  }
  std::string sProtocol = WsjcppCore::extractURLProtocol(sValue);
  if (!WsjcppValidators::isValidURLProtocol(sProtocol, sError)) {
    return false;
  }

  int nStartPos = sProtocol.length() + 3;
  std::string sAuthorityAddressPath = "";
  for (int i = nStartPos; i < sValue.size(); i++) {
    char c = sValue[i];
    if (c == '?') {
      break;
    }
    sAuthorityAddressPath += c;
  }
  std::string sQuery =
      sValue.substr(sProtocol.length() + 3 + sAuthorityAddressPath.size());
  std::string sAddressAndPath = sAuthorityAddressPath;

  int nPosAuthority = sAuthorityAddressPath.find("@");
  std::string sAuthority = "";
  if (nPosAuthority != std::string::npos) {
    sAuthority = sAuthorityAddressPath.substr(0, nPosAuthority);
    sAddressAndPath = sAuthorityAddressPath.substr(nPosAuthority + 1);
  }
  if (sAuthority != "") {
    // sError = "TODO check username and password sAuthority= [" + sAuthority +
    // "]";
    // -.~_!$&'()*+,;=:%40:80%2f::::::
    // WsjcppLog::warn(TAG, sError);
    // return false;
  }
  std::string sAddress = sAddressAndPath;
  std::string sPath = "";
  int nPosAddress = sAddressAndPath.find("/");
  if (nPosAddress != std::string::npos) {
    sAddress = sAddressAndPath.substr(0, nPosAddress);
    sPath = sAddressAndPath.substr(nPosAddress);
  }

  if (sAddress.size() == 0) {
    sError = "Address could not be empty";
    return false;
  }

  int nPosPort = sAddress.find(":");
  std::string sPort = "";
  if (sAddress.find(":") != std::string::npos) {
    sPort = sAddress.substr(nPosPort + 1);
    sAddress = sAddress.substr(0, nPosPort);
  }

  if (sPort != "") {
    if (!WsjcppValidators::isValidPort(sPort, sError)) {
      return false;
    }
  }

  if (std::regex_match(sAddress, m_rxLikeIPv4Format)) {
    if (!WsjcppValidators::isValidIPv4(sAddress, sError)) {
      return false;
    }
    if (sPort != "") {
      int nPort = std::atoi(sPort.c_str());
      if (nPort < 1 || nPort > 65535) {
        sError = "Port '" + std::to_string(nPort) +
                 "' must be more then 0 and less then 65536";
        return false;
      }
    }
  } else {
    if (!WsjcppValidators::isValidDomainName(sAddress, sError)) {
      return false;
    }
  }

  if ((sPath == "" || sPath == "/") && sQuery == "") {
    return true;
  }

  if (sPath != "") {
    for (int i = 0; i < sPath.length(); i++) {
      char c = sPath[i];
      if (c == ' ') {
        sError = "Path could not contains whitespace ' '";
        return false;
      }
    }
  }

  if (sQuery != "") {
    for (int i = 0; i < sQuery.length(); i++) {
      char c = sQuery[i];
      if (c == ' ') {
        sError = "Query could not contains whitespace ' ' (must be encoded)";
        return false;
      }
    }
  }

  // sError = "sAddressAndPath=[" + sAddressAndPath + "], , sAddress=[" +
  // sAddress + "]";
  return true;
}

// ----------------------------------------------------------------------
// WsjcppValidatorDomainName

WsjcppValidatorDomainName::WsjcppValidatorDomainName() : WsjcppValidatorStringBase("domain_name") {
  TAG = "WsjcppValidatorDomainName";
}

bool WsjcppValidatorDomainName::isValid(const std::string &sValue, std::string &sError) {
    return WsjcppValidators::isValidDomainName(sValue, sError);
}

// ----------------------------------------------------------------------
// WsjcppValidatorBase64

WsjcppValidatorBase64::WsjcppValidatorBase64()
    : WsjcppValidatorStringBase("base64") {
  TAG = "WsjcppValidatorBase64";
}

// ----------------------------------------------------------------------

bool WsjcppValidatorBase64::isValid(const std::string &sValue,
                                    std::string &sError) {
  return WsjcppValidators::isValidBase64(sValue, sError);
}

// ----------------------------------------------------------------------
// WsjcppValidatorNumber

WsjcppValidatorNumber::WsjcppValidatorNumber()
    : WsjcppValidatorStringBase("number") {
  TAG = "WsjcppValidatorNumber";
}

// ----------------------------------------------------------------------

bool WsjcppValidatorNumber::isValid(const std::string &sValue,
                                    std::string &sError) {
  int nSize = sValue.size();
  bool bHasOneAndMoreNumbers = false;
  for (int i = 0; i < nSize; i++) {
    char c = sValue[i];
    if (c == '-' && i == 0) {
      continue;
    }
    if (c < '0' || c > '9') {
      sError = "Unexpected char '";
      sError += c;
      sError += "' in " + std::to_string(i) + " position";
      return false;
    }
    bHasOneAndMoreNumbers = true;
  }
  return bHasOneAndMoreNumbers;
}

// ----------------------------------------------------------------------
// WsjcppValidatorHex

WsjcppValidatorHex::WsjcppValidatorHex() : WsjcppValidatorStringBase("hex") {
  TAG = "WsjcppValidatorHex";
}

// ----------------------------------------------------------------------

bool WsjcppValidatorHex::isValid(const std::string &sValue,
                                 std::string &sError) {
  int nSize = sValue.size();
  if (nSize == 0) {
    sError = "Empty string";
    return false;
  }
  for (int i = 0; i < nSize; i++) {
    char c = sValue[i];
    if (c == '-' && i == 0) {
      continue;
    }
    if ((c < '0' || c > '9') && (c < 'a' || c > 'f') && (c < 'A' || c > 'F')) {
      sError = "Unexpected char '";
      sError += c;
      sError += "' in " + std::to_string(i) + " position";
      return false;
    }
  }
  return true;
}

// ----------------------------------------------------------------------
// WsjcppValidatorIntegerBase

WsjcppValidatorIntegerBase::WsjcppValidatorIntegerBase(
    const std::string &sTypeName) {
  TAG = "WsjcppValidatorIntegerBase";
  m_sTypeName = sTypeName;
}

// ----------------------------------------------------------------------

WsjcppValidatorType WsjcppValidatorIntegerBase::getBaseType() {
  return WsjcppValidatorType::WSJCPP_VALIDATOR_INTEGER;
}

// ----------------------------------------------------------------------

std::string WsjcppValidatorIntegerBase::getTypeName() { return m_sTypeName; }

// ----------------------------------------------------------------------
// WsjcppValidatorIntegerMinValue

WsjcppValidatorIntegerMinValue::WsjcppValidatorIntegerMinValue(int nMinValue)
    : WsjcppValidatorIntegerBase("integer_min_value") {
  TAG = "WsjcppValidatorIntegerMinValue";
  m_nMinValue = nMinValue;
}

// ----------------------------------------------------------------------

bool WsjcppValidatorIntegerMinValue::isValid(int nValue, std::string &sError) {
  if (nValue < m_nMinValue) {
    sError = "Value must be more or equal then " + std::to_string(m_nMinValue);
    return false;
  }
  return true;
}

// ----------------------------------------------------------------------
// WsjcppValidatorIntegerMaxValue

WsjcppValidatorIntegerMaxValue::WsjcppValidatorIntegerMaxValue(int nMaxValue)
    : WsjcppValidatorIntegerBase("integer_max_value") {
  TAG = "WsjcppValidatorIntegerMaxValue";
  m_nMaxValue = nMaxValue;
}

// ----------------------------------------------------------------------

bool WsjcppValidatorIntegerMaxValue::isValid(int nValue, std::string &sError) {
  if (nValue > m_nMaxValue) {
    sError = "Value must be less or equal then " + std::to_string(m_nMaxValue);
    return false;
  }
  return true;
}

