#pragma once
#include <string>
#include <vector>
#include <memory>
#include "currency.h"

std::vector<std::shared_ptr<FiatCurrency>> loadFiatCurrencies(const std::string& filename);
std::vector<std::shared_ptr<CryptoCurrency>> loadCryptoCurrencies(const std::string& filename);
std::vector<std::shared_ptr<MagicCurrency>> loadMagicCurrencies(const std::string& filename);

std::vector<std::string> splitCSVLine(const std::string& line, char delimiter = ',');
std::vector<std::string> splitCountries(const std::string& countriesStr);