#pragma once
#include <string>

std::string normalizeCode(const std::string& code);
std::string normalizeCountry(const std::string& country);
bool isString(const std::string& s);
std::string readValidatedString(const std::string& prompt, bool (*validator)(const std::string&));
double readPositiveDouble(const std::string& prompt);