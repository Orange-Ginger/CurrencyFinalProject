#include "currency.h"
#include "text_formatting.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iostream>

// --- Currency base class ---
Currency::Currency(const std::string& _code, const std::string& _symbol, 
                   const std::string& _type, const double _rate, const double _taxRate)
    : code(_code), symbol(_symbol), type(_type), rate(_rate), taxRate(_taxRate) {}

std::string Currency::getCode() const { return code; }
std::string Currency::getSymbol() const { return symbol; }
std::string Currency::getType() const { return type; }
double Currency::getRate() const { return rate; }
double Currency::getTaxRate() const { return taxRate; }

// --- CryptoCurrency ---
CryptoCurrency::CryptoCurrency(const std::string& _code, const std::string& _symbol,
    const double _rate, const double _taxRate, double _volatility,
    const std::vector<std::string>& _allowedCountries)
    : Currency(_code, _symbol, "Crypto", _rate, _taxRate),
      volatility(_volatility), allowedCountries(_allowedCountries) {}

void CryptoCurrency::fluctuate() {
    rate *= volatility;
}

double CryptoCurrency::applyTaxOrFee(double amount) const {
    return amount * taxRate;
}

std::string CryptoCurrency::makeReport(double amount, const std::string& country) const {
    std::string volatilityLevel;
    if (volatility < 1.01)
        volatilityLevel = "Low";
    else if (volatility < 1.05)
        volatilityLevel = "Medium";
    else
        volatilityLevel = "High";

    std::ostringstream info;
    info << std::fixed << std::setprecision(3);
    info << "-- Currency info --\n"
         << code << " " << symbol << " (" + type + ")\n"
         << "Current rate to USD: " << rate << "\n"
         << "Tax on amount: " << (applyTaxOrFee(amount)) << " " << code << "\n"
         << "Volatility: " << volatilityLevel << "\n"
         << "Stable: " << (isStable() ? "Yes" : "No") << "\n"
         << "Allowed in " << country << ": "
         << (canBeUsedIn(country) ? "Yes" : "No") << "\n";
    return info.str();
}

bool CryptoCurrency::isStable() const {
    return volatility < 1.01;
}

bool CryptoCurrency::canBeUsedIn(const std::string& country) const {
    for (const std::string& allowed : allowedCountries) {
        if (allowed == country) return true;
    }
    return false;
}

double CryptoCurrency::getVolatility() const { return volatility; }

// --- FiatCurrency ---
FiatCurrency::FiatCurrency(const std::string& _code, const std::string& _symbol,
    const double _rate, const double _taxRate, const double _inflationRate,
    const std::vector<std::string>& _allowedCountries)
    : Currency(_code, _symbol, "Fiat", _rate, _taxRate),
      allowedCountries(_allowedCountries), inflationRate(_inflationRate) {}

void FiatCurrency::fluctuate() {
    rate *= (1 + inflationRate);
}

double FiatCurrency::applyTaxOrFee(double amount) const {
    if (amount < 100) return amount * taxRate;
    else if (amount < 1000) return amount * (taxRate * 0.95);
    else if (amount < 10000) return amount * (taxRate * 0.9);
    else return amount * (taxRate * 0.8);
}

std::string FiatCurrency::makeReport(double amount, const std::string& country) const {
    std::ostringstream info;
    info << std::fixed << std::setprecision(3);
    info << "-- Currency info --\n"
         << code << " " << symbol << " (" + type + ")\n"
         << "Current rate to USD: " << rate << "\n"
         << "Fee on amount: " << (applyTaxOrFee(amount)) << " " << code << "\n"
         << (inflationRate < 0 ? "Deflation rate: " : "Inflation rate: ")
         << inflationRate * 100 << "%\n"
         << "Stable: " << (isStable() ? "Yes" : "No") << "\n"
         << "Allowed in " << country << ": "
         << (canBeUsedIn(country) ? "Yes" : "No") << "\n";
    return info.str();
}

bool FiatCurrency::isStable() const {
    return inflationRate < 0.03;
}

bool FiatCurrency::canBeUsedIn(const std::string& country) const {
    for (const std::string& allowed : allowedCountries) {
        if (allowed == country) return true;
    }
    return false;
}

double FiatCurrency::getInflationRate() const { return inflationRate; }

// --- MagicCurrency ---
MagicCurrency::MagicCurrency(const std::string& _code, const std::string& _symbol,
    const double _rate, const double _taxRate,
    const int _rarityLevel, const std::string& _incantation,
    const std::string& _realmOrigin)
    : Currency(_code, _symbol, "Magic", _rate, _taxRate),
      rarityLevel(_rarityLevel), incantation(_incantation), realmOrigin(_realmOrigin) {}

void MagicCurrency::fluctuate() {
    std::uniform_real_distribution<> rng(-0.2, 0.3);
    double changeFactor = 1.0 + rng(gen);
    if (rarityLevel > 5) {
        if (changeFactor < 1.0) changeFactor = 1.0 + (rng(gen) / 2);
    }
    rate *= changeFactor;
}

double MagicCurrency::applyTaxOrFee(double amount) const {
    int power = 0;
    for (char c : incantation) {
        power += static_cast<int>(c);
    }
    double modifier = 1.0;
    if (power % 2 == 0) modifier *= 0.1;
    else modifier *= 1.5;
    return amount * taxRate * modifier;
}

std::string MagicCurrency::makeReport(double amount, const std::string& country) const {
    std::ostringstream info;
    info << std::fixed << std::setprecision(3);
    info << "-- Currency info --\n"
         << code << " " << symbol << " (" + type + ")\n"
         << "Current rate to USD: " << rate << "\n"
         << "Fee on amount: " << (applyTaxOrFee(amount)) << " " << code << "\n"
         << "Rarity level: " << rarityLevel << "\n"
         << "Magic incantation: " << "\033[3m" << incantation << "\033[0m" << "\n"
         << "Realm of origin: " << realmOrigin << "\n"
         << "Stable: " << (isStable() ? "Yes" : "No") << " (of course it isn't)" << "\n"
         << "Allowed in " << country << ": "
         << (canBeUsedIn(country) ? "Yes" : "No") << "\n";
    return info.str();
}

bool MagicCurrency::isStable() const {
    return false;
}

bool MagicCurrency::canBeUsedIn(const std::string& country) const {
    if (country == realmOrigin) return true;
    else {
        std::uniform_int_distribution<int> rng(1, 10);
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += rng(gen);
        }
        if (sum > 50) return true;
    }
    return false;
}

int MagicCurrency::getRarityLevel() const { return rarityLevel; }
std::string MagicCurrency::getIncantation() const { return incantation; }
std::string MagicCurrency::getRealmOrigin() const { return realmOrigin; }

// --- CurrencyConverter ---
CurrencyConverter::CurrencyConverter() = default;

void CurrencyConverter::addCurrency(const std::shared_ptr<Currency>& currency) {
    currencies[normalizeCode(currency->getCode())] = currency;
}

std::shared_ptr<Currency> CurrencyConverter::getCurrency(const std::string& code) const {
    auto it = currencies.find(normalizeCode(code));
    if (it == currencies.end()) throw std::runtime_error("Currency '" + code + "' not found.");
    return it->second;
}

double CurrencyConverter::convert(const std::string& fromCode, const std::string& toCode, double amount) const {
    auto fromIt = currencies.find(normalizeCode(fromCode));
    auto toIt = currencies.find(normalizeCode(toCode));

    if (fromIt == currencies.end()) throw std::runtime_error("Currency '" + fromCode + "' not found.");
    if (toIt == currencies.end()) throw std::runtime_error("Currency '" + toCode + "' not found.");

    double amountInUSD = amount * fromIt->second->getRate();
    double converted = amountInUSD / toIt->second->getRate();
    return converted;
}

std::string CurrencyConverter::getReport(const std::string& code, double amount, const std::string& country) const {
    auto it = currencies.find(normalizeCode(code));
    if (it == currencies.end()) throw std::runtime_error("Currency '" + code + "' not found.");
    return it->second->makeReport(amount, normalizeCountry(country));
}

void CurrencyConverter::fluctuateAll() {
    for (auto& pair : currencies) {
        pair.second->fluctuate();
    }
}

void CurrencyConverter::listAllCurrencies() const {
    std::vector<std::shared_ptr<Currency>> currencyList;
    for (const auto& pair : currencies) {
        currencyList.push_back(pair.second);
    }
    std::sort(currencyList.begin(), currencyList.end(),
             [](const std::shared_ptr<Currency>& a,
                const std::shared_ptr<Currency>& b) {
                if (a->getType() == b->getType())
                    return a->getCode() < b->getCode();
                return a->getType() < b->getType();
             });
    std::cout << "Available currencies:\n";
    for (const auto& currency : currencyList) {
        std::cout << "-" << currency->getCode() << " " << currency->getSymbol()
                  << " (" << currency->getType() << ")\n";
    }
}