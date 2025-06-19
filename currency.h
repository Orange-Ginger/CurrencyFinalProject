#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <random>
#include <iomanip>
#include <unordered_map>
#include <stdexcept>

class Currency {
protected:
    std::string code;
    std::string symbol;
    std::string type;
    double rate;
    double taxRate;
public:
    Currency(const std::string& _code, const std::string& _symbol, 
        const std::string& _type, const double _rate, const double _taxRate) :
    code(_code), symbol(_symbol), type(_type), rate(_rate), taxRate(_taxRate) {}
    
    virtual ~Currency() = default;
    virtual void fluctuate() = 0;
    virtual double applyTaxOrFee(double amount) const = 0;
    virtual std::string makeReport(double amount,
        const std::string& country) const = 0;
    virtual bool isStable() const = 0;
    virtual bool canBeUsedIn(const std::string& country) const = 0;

    std::string getCode() const { return code; }
    std::string getSymbol() const { return symbol; }
    std::string getType() const { return type; }
    double getRate() const { return rate; }
};

class CryptoCurrency : public Currency {
private:
    double volatility;
    std::vector<std::string> allowedCountries;
public:
    CryptoCurrency(const std::string& _code, const std::string& _symbol,
        const double _rate, const double _taxRate, double _volatility) : 
    Currency(_code, _symbol, "Crypto", _rate, _taxRate),
        volatility(_volatility) {}

    virtual void fluctuate() override {
        rate *= volatility;
    }

    virtual double applyTaxOrFee(double amount) const override {
        return amount * taxRate;
    }

    virtual std::string makeReport(double amount,
        const std::string& country) const override {
        std::string volatilityLevel;
        if (volatility < 1.01)
            volatilityLevel = "Low";
        else if (volatility < 1.05)
            volatilityLevel = "Medium";
        else 
            volatilityLevel = "High";

        std::ostringstream info;
        info << std::fixed << std::setprecision(6);
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

    virtual bool isStable() const override {
        return volatility < 1.01;
    }

    virtual bool canBeUsedIn(const std::string& country) const override {
        for (const std::string& allowed : allowedCountries) {
            if (allowed == country) return true;
        }
        return false;
    }

    void setAllowedCountries(const std::vector<std::string>& countries) {
        allowedCountries = countries;
    }
};

class FiatCurrency : public Currency {
private:
    double inflationRate;
    std::vector<std::string> allowedCountries;
public:
    FiatCurrency(const std::string& _code, const std::string& _symbol,
        const double _rate, const double _taxRate, const double _inflationRate) : 
    Currency(_code, _symbol, "Fiat", _rate, _taxRate),
        inflationRate(_inflationRate) {}

    virtual void fluctuate() override {
        rate *= (1 + inflationRate);
    }

    virtual double applyTaxOrFee(double amount) const override {
        if (amount < 100) return amount * taxRate;
        else if (amount < 1000) return amount * (taxRate * 0.95);
        else if (amount < 10000) return amount * (taxRate * 0.9);
        else return amount * (taxRate * 0.8);
    }

    virtual std::string makeReport(double amount,
        const std::string& country) const override {
        std::ostringstream info;
        info << std::fixed << std::setprecision(6);
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

    virtual bool isStable() const override {
        return inflationRate < 0.03;
    }

    virtual bool canBeUsedIn(const std::string& country) const override {
        for (const std::string& allowed : allowedCountries) {
            if (allowed == country) return true;
        }
        return false;
    }

    void setAllowedCountries(const std::vector<std::string>& countries) {
        allowedCountries = countries;
    }
};

class MagicCurrency : public Currency {
protected:
    int rarityLevel;
    std::string incantation;
    std::string realmOrigin;
    static inline std::random_device rd;
    static inline std::mt19937 gen;
public:
    MagicCurrency(const std::string& _code, const std::string& _symbol,
        const std::string& _type, const double _rate, const double _taxRate,
        const int _rarityLevel, const std::string& _incantation,
        const std::string& _realmOrigin) :
    Currency(_code, _symbol, "Magic", _rate, _taxRate), rarityLevel(_rarityLevel),
        incantation(_incantation), realmOrigin(_realmOrigin) {}

    virtual void fluctuate() override {
        std::uniform_real_distribution<> rng(-0.2, 0.3);
        double changeFactor = 1.0 + rng(gen);
        if (rarityLevel > 5) {
            if (changeFactor < 1.0) changeFactor = 1.0 + (rng(gen) / 2);
        }
        rate *= changeFactor;
    }

    virtual double applyTaxOrFee(double amount) const override {
        int power = 0;
        for (char c : incantation) {
            power += static_cast<int>(c);
        }
        double modifier = 1.0;
        if (power % 2 == 0) modifier *= 0.1;
        else modifier *= 1.5;
        return amount * taxRate * modifier;
    }

    virtual std::string makeReport(double amount,
        const std::string& country) const override {
        std::ostringstream info;
        info << std::fixed << std::setprecision(6);
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

    virtual bool isStable() const override {
        return false;
    }

    virtual bool canBeUsedIn(const std::string& country) const override {
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
};

class CurrencyConverter {
private:
    std::unordered_map<std::string, std::shared_ptr<Currency>> currencies;
public:
    void addCurrency(std::shared_ptr<Currency>& currency) {
        currencies[currency->getCode()] = currency;
    }

    double convert(const std::string& fromCode, const std::string& toCode,
        double amount) const {
        auto fromIt = currencies.find(fromCode);
        auto toIt = currencies.find(toCode);

        if (fromIt == currencies.end()) throw std::runtime_error("Currency '" +
            fromCode + "' not found.");
        if (toIt == currencies.end()) throw std::runtime_error("Currency '" +
            toCode + "' not found.");

        double amountInUSD = amount / fromIt->second->getRate();
        double converted = amount * toIt->second->getRate();
        return converted;
    }

    std::string getReport(const std::string& code, double amount,
        const std::string& country) const {
        auto it = currencies.find(code);
        if (it == currencies.end()) throw std::runtime_error("Currency '" +
            code + "' not found.");
        return it->second->makeReport(amount, country);
    }

    void fluctuateAll() {
        for (auto& pair : currencies) {
            pair.second->fluctuate();
        }
    }

    void listAllCurrencies() const {
        std::cout << "Available currencies:\n";
        for (const auto& pair : currencies) {
            std::cout << "-" << pair.first << pair.second->getSymbol()
            <<" (" << pair.second->getType() << ")\n";
        }
    }
};