#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "currency_parser.h"
#include "currency.h"

std::vector<std::string> splitCSVLine(const std::string& line, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string token;
    bool insideQuotes = false;
    std::string field;

    for (char ch : line) {
        if (ch == '"') insideQuotes = !insideQuotes;
        else if (ch == delimiter && !insideQuotes) {
            result.push_back(field);
            field.clear();
        } else field += ch;
    }
    result.push_back(field);
    return result;
}

std::vector<std::string> splitCountries(const std::string& countriesStr) {
    std::vector<std::string> countries;
    std::stringstream ss(countriesStr);
    std::string country;
    while (std::getline(ss, country, ',')) {
        countries.push_back(country);
    }
    return countries;
}

std::vector<std::shared_ptr<CryptoCurrency>> loadCryptoCurrencies(const std::string& filename) {
    std::vector<std::shared_ptr<CryptoCurrency>> currencies;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Couldn't open the file " << filename << "\n";
        return currencies;
    }

    std::string line;
    std::getline(file, line); //skip the header

    while (std::getline(file, line)) {
        auto fields = splitCSVLine(line);
        if (fields.size() < 6) continue;

        try {
            std::string code = fields[0];
            std::string symbol = fields[1];
            double rate = std::stod(fields[2]);
            double taxRate = std::stod(fields[3]);
            double volatility = std::stod(fields[4]);
            auto allowedCountries = splitCountries(fields[5]);

            auto currency = std::make_shared<CryptoCurrency>(code, symbol,
                rate, taxRate, volatility, allowedCountries);
            currencies.push_back(currency);

        } catch(const std::exception& exception) {
            std::cerr << "Error parsing line: \"" << line << "\" - "
            << exception.what() << "\n";
        }
    }
    return currencies;
}

std::vector<std::shared_ptr<FiatCurrency>> loadFiatCurrencies(const std::string& filename) {
    std::vector<std::shared_ptr<FiatCurrency>> currencies;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Couldn't open the file " << filename << "\n";
        return currencies;
    }

    std::string line;
    std::getline(file, line); //skip the header

    while (std::getline(file, line)) {
        auto fields = splitCSVLine(line);
        if (fields.size() < 6) continue;

        try {
            std::string code = fields[0];
            std::string symbol = fields[1];
            double rate = std::stod(fields[2]);
            double taxRate = std::stod(fields[3]);
            double inflationRate = std::stod(fields[4]);
            auto allowedCountries = splitCountries(fields[5]);

            auto currency = std::make_shared<FiatCurrency>(code, symbol,
                rate, taxRate, inflationRate, allowedCountries);
            currencies.push_back(currency);

        } catch(const std::exception& exception) {
            std::cerr << "Error parsing line: \"" << line << "\" - "
            << exception.what() << "\n";
        }
    }
    return currencies;
}

std::vector<std::shared_ptr<MagicCurrency>> loadMagicCurrencies(const std::string& filename) {
    std::vector<std::shared_ptr<MagicCurrency>> currencies;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Couldn't open the file " << filename << "\n";
        return currencies;
    }

    std::string line;
    std::getline(file, line); //skip the header

    while (std::getline(file, line)) {
        auto fields = splitCSVLine(line);
        if (fields.size() < 7) continue;

        try {
            std::string code = fields[0];
            std::string symbol = fields[1];
            double rate = std::stod(fields[2]);
            double taxRate = std::stod(fields[3]);
            int rarityLevel = std::stoi(fields[4]);
            std::string incantation = fields[5];
            std::string realmOrigin = fields[6];

            auto currency = std::make_shared<MagicCurrency>(code, symbol,
                rate, taxRate, rarityLevel, incantation, realmOrigin);
            currencies.push_back(currency);

        } catch(const std::exception& exception) {
            std::cerr << "Error parsing line: \"" << line << "\" - "
            << exception.what() << "\n";
        }
    }
    return currencies;
}