#include "wsjcpp_geoip.h"
#include <json.hpp>
#include <wsjcpp_core.h>
#include <wsjcpp_validators.h>
#include <curl/curl.h>

// ----------------------------------------------------------------------

WsjcppGeoIPResult::WsjcppGeoIPResult(
    const std::string &sServiceName,
    const std::string &sIpAddress,
    const std::string &sCountry,
    const std::string &sRegionName,
    const std::string &sCity,
    double nLatitude,
    double nLongitude
) {
    TAG = "WsjcppGeoIPResult-" + sServiceName;
    m_bHasError = false;
    m_sErrorDescription = "";
    m_sServiceName = sServiceName;
    m_sIpAddress = sIpAddress;
    m_sCountry = sCountry;
    m_sRegionName = sRegionName;
    m_sCity = sCity;
    m_nLatitude = nLatitude;
    m_nLongitude = nLongitude;
}

// ----------------------------------------------------------------------

WsjcppGeoIPResult::WsjcppGeoIPResult(
    const std::string &sServiceName,
    const std::string &sIpAddress,
    const std::string &sErrorDescription
) {
    TAG = "WsjcppGeoIPResult-" + sServiceName;
    m_bHasError = true;
    m_sIpAddress = sIpAddress;
    m_sErrorDescription = sErrorDescription;
    m_sServiceName = sServiceName;
    m_sCountry = "";
    m_sRegionName = "";
    m_sCity = "";
    m_nLatitude = 0;
    m_nLongitude = 0;
}

// ----------------------------------------------------------------------

bool WsjcppGeoIPResult::hasError() {
    return m_bHasError;
}

// ----------------------------------------------------------------------

std::string WsjcppGeoIPResult::getServiceName() {
    return m_sServiceName;
}

// ----------------------------------------------------------------------

std::string WsjcppGeoIPResult::getIpAddress() {
    return m_sIpAddress;
}

// ----------------------------------------------------------------------

std::string WsjcppGeoIPResult::getErrorDescription() {
    if (!m_bHasError) {
        WsjcppLog::warn(TAG, "getErrorDescription, Result hasn't error");
    }
    return m_sErrorDescription;
}

// ----------------------------------------------------------------------

std::string WsjcppGeoIPResult::getCountry() {
    if (m_bHasError) {
        WsjcppLog::warn(TAG, "getCountry, result has error");
    }
    return m_sCountry;
}

// ----------------------------------------------------------------------

std::string WsjcppGeoIPResult::getRegionName() {
    if (m_bHasError) {
        WsjcppLog::warn(TAG, "getRegionName, result has error");
    }
    return m_sRegionName;
}

// ----------------------------------------------------------------------

std::string WsjcppGeoIPResult::getCity() {
    if (m_bHasError) {
        WsjcppLog::warn(TAG, "getCity, result has error");
    }
    return m_sCity;
}

// ----------------------------------------------------------------------

double WsjcppGeoIPResult::getLatitude() {
    if (m_bHasError) {
        WsjcppLog::warn(TAG, "getLatitude, result has error");
    }
    return m_nLatitude;
}

// ----------------------------------------------------------------------

double WsjcppGeoIPResult::getLongitude() {
    if (m_bHasError) {
        WsjcppLog::warn(TAG, "getLongitude, result has error");
    }
    return m_nLongitude;
}

// ----------------------------------------------------------------------

nlohmann::json WsjcppGeoIPResult::toJson() {
    nlohmann::json jsonRet;
    jsonRet["serviceName"] = m_sServiceName;
    jsonRet["ip"] = m_sIpAddress;
    if (m_bHasError) {
        jsonRet["error"] = m_sErrorDescription;
    } else {
        jsonRet["longitude"] = m_nLongitude;
        jsonRet["latitude"] = m_nLatitude;
        jsonRet["longitude"] = m_sCountry;
        jsonRet["regionName"] = m_sRegionName;
        jsonRet["city"] = m_sCity;
    }
    return jsonRet;
}

// ----------------------------------------------------------------------
// WsjcppGeoIPIPv4

WsjcppGeoIPv4::WsjcppGeoIPv4() {
    m_arrIP[0] = 0;
    m_arrIP[1] = 0;
    m_arrIP[2] = 0;
    m_arrIP[3] = 0;
}

// ----------------------------------------------------------------------

WsjcppGeoIPv4::WsjcppGeoIPv4(const unsigned char arrIp[4]) {
    for (int i = 0; i < 4; i++) {
        m_arrIP[i] = arrIp[i];
    }
}

// ----------------------------------------------------------------------

WsjcppGeoIPv4::WsjcppGeoIPv4(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3) {
    m_arrIP[0] = c0;
    m_arrIP[1] = c1;
    m_arrIP[2] = c2;
    m_arrIP[3] = c3;
}

// ----------------------------------------------------------------------

bool WsjcppGeoIPv4::fromString(const std::string &sIpAddress) {
    int n = 0;
    std::string s[4] = {"", "", "", ""};
    for (int i = 0; i < sIpAddress.length(); i++) {
        char c = sIpAddress[i];
        if (n > 3) {
            return false;
        }
        if (c >= '0' && c <= '9') {
            s[n] += c;
        } else if (c == '.') {
            n++;
        } else {
            return false;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (s[i].length() > 3) {
            return false;
        }
        int p = std::stoi(s[i]);
        if (p > 255 || p < 0) {
            return false;
        }
        m_arrIP[i] = p;
    }
    return true;
}

// ----------------------------------------------------------------------

std::string WsjcppGeoIPv4::toString() {
    std::string sRet = 
        std::to_string(m_arrIP[0]) + "."
        + std::to_string(m_arrIP[1]) + "."
        + std::to_string(m_arrIP[2]) + "."
        + std::to_string(m_arrIP[3]);
    return sRet;
}

// ----------------------------------------------------------------------

bool WsjcppGeoIPv4::operator==(const WsjcppGeoIPv4& rhs) {
    for (int i = 0; i < 4; i++) {
        if (m_arrIP[i] == rhs.m_arrIP[i]) {
            continue;
        }
        if (m_arrIP[i] != rhs.m_arrIP[i]) {
            return false;
        }
    }
    return true;
}

// ----------------------------------------------------------------------

bool WsjcppGeoIPv4::operator<(const WsjcppGeoIPv4& rhs) {
    for (int i = 0; i < 4; i++) {
        if (m_arrIP[i] == rhs.m_arrIP[i]) {
            continue;
        }
        if (m_arrIP[i] < rhs.m_arrIP[i]) {
            return true;
        }
    }
    return false;
}

// ----------------------------------------------------------------------

bool WsjcppGeoIPv4::operator>(const WsjcppGeoIPv4& rhs) {
    for (int i = 0; i < 4; i++) {
        if (m_arrIP[i] == rhs.m_arrIP[i]) {
            continue;
        }
        if (m_arrIP[i] > rhs.m_arrIP[i]) {
            return true;
        }
    }
    return false;
}

// ----------------------------------------------------------------------

bool WsjcppGeoIPv4::operator<=(const WsjcppGeoIPv4& rhs) {
    for (int i = 0; i < 4; i++) {
        if (m_arrIP[i] == rhs.m_arrIP[i]) {
            continue;
        }
        if (m_arrIP[i] > rhs.m_arrIP[i]) {
            return false;
        }
    }
    return true;
}

// ----------------------------------------------------------------------

bool WsjcppGeoIPv4::operator>=(const WsjcppGeoIPv4& rhs) {
    for (int i = 0; i < 4; i++) {
        if (m_arrIP[i] == rhs.m_arrIP[i]) {
            continue;
        }
        if (m_arrIP[i] < rhs.m_arrIP[i]) {
            return false;
        }
    }
    return true;
}

// ----------------------------------------------------------------------

size_t WsjcppGeoIP_CallbackFunc_DataToString(char *data, size_t size, size_t nmemb, std::string *writerData) {
    if (writerData == NULL) {
        return 0;
    }
    writerData->append(data, size*nmemb);
    return size * nmemb;
}

// ----------------------------------------------------------------------

WsjcppGeoIPResult WsjcppGeoIP::requestToIpApiCom(const std::string &sIpAddress) {
    std::string sServiceName = "ip-api.com";
    std::string TAG = "requestToIpApiCom";
    std::string sError;
    bool bValid = false;
    if (WsjcppValidators::isValidIPv4(sIpAddress, sError)) {
        bValid = true;
        WsjcppGeoIPv4 ipv4;
        ipv4.fromString(sIpAddress);
        if (WsjcppGeoIP::isIPv4InReservedRange(ipv4, sError)) {
            return WsjcppGeoIPResult(sServiceName, sIpAddress, "reserved range (" + sError + ")");
        }
    } else if (WsjcppValidators::isValidIPv6(sIpAddress, sError)) {
        // TODO check valid range for ipv6
    } else {
        return WsjcppGeoIPResult("ipapi.com", sIpAddress, "Expected ip address");
    }

    std::string sUrl = "http://ip-api.com/json/" + sIpAddress;

    std::string sUserAgent = "wsjcpp-geoip";
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init(); 
    std::string sResponse = "";
    if (curl) { 
        // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //only for https
        // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //only for https
        curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WsjcppGeoIP_CallbackFunc_DataToString); 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sResponse);
        
        curl_easy_setopt(curl, CURLOPT_USERAGENT, sUserAgent.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        res = curl_easy_perform(curl); 
        if (res != CURLE_OK) {
            std::string sError = "Curl failed, reason  " + std::string(curl_easy_strerror(res));
            WsjcppLog::err(TAG, sError); 
            curl_easy_cleanup(curl);
            WsjcppGeoIPResult(sServiceName, sIpAddress, sError);
        } else {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            if (response_code != 200) {
                WsjcppLog::info(TAG, "end " + std::to_string(response_code));
                curl_easy_cleanup(curl);
            }
        }
        // always cleanup
        curl_easy_cleanup(curl);
    }
    return WsjcppGeoIP::parseResponseIpApiCom(sIpAddress, sResponse);
}

// ----------------------------------------------------------------------

WsjcppGeoIPResult WsjcppGeoIP::parseResponseIpApiCom(const std::string &sIpAddress, const std::string &sJson) {
    std::string sServiceName = "ip-api.com";
    std::string TAG = "parseResponseIpApiCom";
    std::string sRequestUrl = "ip-api.com/json/" + sIpAddress;
    std::string sCountry;
    std::string sRegionName;
    std::string sCity;
    double nLatitude;
    double nLongitude;
    std::string sStatus = "";

    if (!nlohmann::json::accept(sJson)) {
        WsjcppLog::err(TAG, sRequestUrl + ", Expected json: " + sJson);
        return WsjcppGeoIPResult(sServiceName, sIpAddress, "Expected json");
    }

    nlohmann::json obj = nlohmann::json::parse(sJson);
    
    try {
        sStatus = obj.at("status").get<std::string>();
    } catch (const std::exception &e) {
        std::string sError = sRequestUrl + " -> wrong field 'status' in struct \n" + sJson + "\n" + std::string(e.what());
        WsjcppLog::err(TAG, sError);
        return WsjcppGeoIPResult(sServiceName, sIpAddress, sError);
    }

    if (sStatus == "fail") {
        std::string sErrorMessage = "";
        try {
            sErrorMessage = obj.at("message").get<std::string>();
        } catch (const std::exception &e) {
            std::string sError = sRequestUrl + " -> wrong field 'message' in struct \n" + sJson + "\n" + std::string(e.what());
            WsjcppLog::err(TAG, sError);
            return WsjcppGeoIPResult(sServiceName, sIpAddress, sError);
        }
        WsjcppLog::err(TAG, sRequestUrl + " -> fail: " + sErrorMessage);
        return WsjcppGeoIPResult(sServiceName, sIpAddress, sErrorMessage);
    }

    try {
        sCountry = obj.at("country").get<std::string>();
    } catch (const std::exception &e) {
        WsjcppLog::err(TAG, sRequestUrl + " -> wrong field 'country' in struct \n" + sJson + "\n" + std::string(e.what()));
        sCountry = "";
    }

    try {
        sRegionName = obj.at("regionName").get<std::string>();
    } catch (const std::exception &e) {
        WsjcppLog::err(TAG, sRequestUrl + " -> wrong field 'regionName' in struct \n" + sJson + "\n" + std::string(e.what()));
        sRegionName = "";
    }

    try {
        sCity = obj.at("city").get<std::string>();
    } catch (const std::exception &e) {
        WsjcppLog::err(TAG, sRequestUrl + " -> wrong field 'city' in struct \n" + sJson + "\n" + std::string(e.what()));
        sCity = "";
    }

    try {
        nLatitude = obj.at("lat").get<double>();
    } catch (const std::exception &e) {
        WsjcppLog::err(TAG, sRequestUrl + " -> wrong field 'lat' in struct \n" + sJson + "\n" + std::string(e.what()));
        nLatitude = 0.0;
    }

    try {
        nLongitude = obj.at("lon").get<double>();
    } catch (const std::exception &e) {
        WsjcppLog::err(TAG, sRequestUrl + " -> wrong field 'lon' in struct \n" + sJson + "\n" + std::string(e.what()));
        nLongitude = 0.0;
    }
    // ok
    return WsjcppGeoIPResult(
        sServiceName, 
        sIpAddress,
        sCountry,
        sRegionName,
        sCity,
        nLatitude,
        nLongitude
    );
}

// ----------------------------------------------------------------------

bool WsjcppGeoIP::isIPv4InReservedRange(const WsjcppGeoIPv4& ipV4, std::string &sError) {
    sError = "";
    WsjcppGeoIPv4 currentIp = ipV4;
    
    // https://en.wikipedia.org/wiki/Reserved_IP_addresses

    if (currentIp >= WsjcppGeoIPv4(0,0,0,0) && currentIp <= WsjcppGeoIPv4(0,255,255,255)) {
        sError = "Software. Current network (only valid as source address).";
        return true;
    }

    if (currentIp >= WsjcppGeoIPv4(10,0,0,0) && currentIp <= WsjcppGeoIPv4(10,255,255,255)) {
        sError = "Private network. Used for local communications within a private network.";
        return true;
    }

    
    if (currentIp >= WsjcppGeoIPv4(100,64,0,0) && currentIp <= WsjcppGeoIPv4(100,127,255,255)) {
        sError = "Private network. Shared address space for communications between a service provider and its subscribers when using a carrier-grade NAT.";
        return true;
    }

    
    if (currentIp >= WsjcppGeoIPv4(127,0,0,0) && currentIp <= WsjcppGeoIPv4(127,255,255,255)) {
        sError = "Host. Used for loopback addresses to the local host.";
        return true;
    }

    
    if (currentIp >= WsjcppGeoIPv4(169,254,0,0) && currentIp <= WsjcppGeoIPv4(169,254,255,255)) {
        sError = "Subnet. Used for link-local addresses between two hosts on a single link when no IP address is otherwise specified, such as would have normally been retrieved from a DHCP server.";
        return true;
    }

    
    if (currentIp >= WsjcppGeoIPv4(172,16,0,0) && currentIp <= WsjcppGeoIPv4(172,31,255,255)) {
        sError = "Private network. Used for local communications within a private network.";
        return true;
    }

    
    if (currentIp >= WsjcppGeoIPv4(192,0,0,0) && currentIp <= WsjcppGeoIPv4(192,0,0,255)) {
        sError = "Private network. IETF Protocol Assignments.";
        return true;
    }

    
    if (currentIp >= WsjcppGeoIPv4(192,0,2,0) && currentIp <= WsjcppGeoIPv4(192,0,2,255)) {
        sError = "Documentation. Assigned as TEST-NET-1, documentation and examples.";
        return true;
    }

    
    if (currentIp >= WsjcppGeoIPv4(192,88,99,0) && currentIp <= WsjcppGeoIPv4(192,88,99,255)) {
        sError = "Internet. Reserved. Formerly used for IPv6 to IPv4 relay (included IPv6 address block 2002::/16).";
        return true;
    }

    
    if (currentIp >= WsjcppGeoIPv4(192,168,0,0) && currentIp <= WsjcppGeoIPv4(192,168,255,255)) {
        sError = "Private network. Used for local communications within a private network.";
        return true;
    }

    
    if (currentIp >= WsjcppGeoIPv4(198,18,0,0) && currentIp <= WsjcppGeoIPv4(198,19,255,255)) {
        sError = "Private network. Used for benchmark testing of inter-network communications between two separate subnets.";
        return true;
    }

    // 
    // if (currentIp >= WsjcppGeoIPv4(198,51,100,0) && currentIp <= WsjcppGeoIPv4(198,51,100,255)) {
    //     sError = "Documentation. Assigned as TEST-NET-2, documentation and examples.";
    //     return true;
    // }

    // if (currentIp >= WsjcppGeoIPv4(203,0,113,0) && currentIp <= WsjcppGeoIPv4(203,0,113,255)) {
    //     sError = "Documentation. Assigned as TEST-NET-3, documentation and examples.";
    //     return true;
    // }
    
    if (currentIp >= WsjcppGeoIPv4(224,0,0,0) && currentIp <= WsjcppGeoIPv4(239,255,255,255)) {
        sError = "Internet. In use for IP multicast. (Former Class D network).";
        return true;
    }

    if (currentIp >= WsjcppGeoIPv4(240,0,0,0) && currentIp <= WsjcppGeoIPv4(255,255,255,254)) {
        sError = "Internet. Reserved for future use. (Former Class E network).";
        return true;
    }

    if (currentIp >= WsjcppGeoIPv4(240,0,0,0) && currentIp <= WsjcppGeoIPv4(255,255,255,254)) {
        sError = "Internet. Reserved for future use. (Former Class E network).";
        return true;
    }
    
    if (currentIp >= WsjcppGeoIPv4(255,255,255,255) && currentIp <= WsjcppGeoIPv4(255,255,255,255)) {
        sError = "Subnet. Reserved for the 'limited broadcast' destination address.";
        return true;
    }

    return false;
}
