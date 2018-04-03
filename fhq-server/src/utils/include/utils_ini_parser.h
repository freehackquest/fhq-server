#ifndef UTILS_INI_PARSER_H
#define UTILS_INI_PARSER_H

#include <stdio.h>
#include <string>

class UtilsIniParser {
	public:
		UtilsIniParser();
		void parse(const std::wstring &sFilename);
};

#endif // UTILS_INI_PARSER_H
