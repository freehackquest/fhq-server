#include "unit_test_parser_ip_api_com.h"
#include <vector>
#include <iostream>
#include <fallen.h>
#include <parser_ip_api_com.h>

REGISTRY_UNIT_TEST(UnitTestParserIpApiCom)


UnitTestParserIpApiCom::UnitTestParserIpApiCom()
    : UnitTestBase("UnitTestParserIpApiCom") {
    //
}

void UnitTestParserIpApiCom::init() {
    // nothing
}

bool UnitTestParserIpApiCom::run() {

    std::string msg1 = "{\"status\":\"fail\",\"message\":\"reserved range\",\"query\":\"127.0.0.1\"}";
    std::string msg2 = "{\"status\":\"success\",\"country\":\"Russia\",\"countryCode\":\"RU\",\"region\":\"MOW\",\"regionName\":\"Moscow\",\"city\":\"Moscow\",\"zip\":\"144700\",\"lat\":55.7737,\"lon\":37.6055,\"timezone\":\"Europe/Moscow\",\"isp\":\"Net By Net Holding LLC\",\"org\":\"Net By Net Holding LLC\",\"as\":\"AS12714 Net By Net Holding LLC\",\"query\":\"79.120.78.1\"}";
    std::string msg3 = "{\"status\":\"success\",\"country\":\"Russia\",\"countryCode\":\"RU\",\"region\":\"MOW\",\"regionName\":\"Moscow\",\"city\":\"Moscow\",\"zip\":\"101194\",\"lat\":55.7315,\"lon\":37.6457,\"timezone\":\"Europe/Moscow\",\"isp\":\"CORBINA\",\"org\":\"\",\"as\":\"AS16345 Public Joint Stock Company \\\"Vimpel-Communications\\\"\",\"query\":\"213.234.222.81\"}";
    
    ParserIpApiCom parser;
    bool bTestSuccess = true;
    compareB(bTestSuccess, "msg1_status_fail", parser.parse("127.0.0.1", msg1), false);

    compareB(bTestSuccess, "msg2_status_success", parser.parse("79.120.78.1", msg2), true);
    compareS(bTestSuccess, "msg2_country", parser.getCountry(), "Russia");
    compareS(bTestSuccess, "msg2_region", parser.getRegion(), "Moscow");
    compareS(bTestSuccess, "msg2_city", parser.getCity(), "Moscow");
    compareD(bTestSuccess, "msg2_lat", parser.getLat(), 55.7737);
    compareD(bTestSuccess, "msg2_lon", parser.getLon(), 37.6055);

    compareB(bTestSuccess, "msg3_status_success", parser.parse("213.234.222.81", msg3), true);
    compareS(bTestSuccess, "msg3_country", parser.getCountry(), "Russia");
    compareS(bTestSuccess, "msg3_region", parser.getRegion(), "Moscow");
    compareS(bTestSuccess, "msg3_city", parser.getCity(), "Moscow");
    compareD(bTestSuccess, "msg3_lat", parser.getLat(), 55.7315);
    compareD(bTestSuccess, "msg3_lon", parser.getLon(), 37.6457);

    return bTestSuccess;
}

