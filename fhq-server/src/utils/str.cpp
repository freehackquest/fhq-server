#include "str.h"
#include <string>
#include <algorithm>

// ---------------------------------------------------------------------

std::string& Str::ltrim(std::string& str, const std::string& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

// ---------------------------------------------------------------------

std::string& Str::rtrim(std::string& str, const std::string& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

// ---------------------------------------------------------------------

std::string& Str::trim(std::string& str, const std::string& chars) {
    return Str::ltrim(Str::rtrim(str, chars), chars);
}

// ---------------------------------------------------------------------

std::string& Str::to_lower(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

// ---------------------------------------------------------------------