#pragma once
#include <iostream>
#include <vector>
#include <sstream>

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
    virtual double getRate() const = 0;
    virtual void fluctuate() = 0;
    virtual double applyTaxOrFee(double amount) const = 0;
    virtual std::string makeReport(double amount,
        const std::string& country) const = 0;
    virtual bool isStable() const = 0;
    virtual bool canBeUsedIn(const std::string& country) const = 0;

    std::string getCode() const { return code; }
    std::string getSymbol() const { return symbol; }
    std::string getType() const { return type; }
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

    virtual double getRate() const override {
        return rate;
    }

    virtual void fluctuate() override {
        rate *= volatility;
    }

    virtual double applyTaxOrFee(double amount) const override {
        return amount * 0.02;
    }

    virtual bool isStable() const override {
        return volatility < 1.01;
    }

    virtual bool canBeUsedIn(const std::string& country) const override {
        for (const auto& allowed : allowedCountries) {
            if (allowed == country) return true;
        }
        return false;
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
        info << "-- Currency info --\n"
             << code << symbol << " (" + type + ")\n"
             << "Current rate to USD: " << rate << "\n"
             << "Tax on amount: " << (applyTaxOrFee(amount)) << " " << code << "\n"
             << "Volatility: " << volatilityLevel << "\n"
             << "Stable: " << (isStable() ? "Yes" : "No") << "\n"
             << "Allowed in " << country << ": " 
             << (canBeUsedIn(country) ? "Yes" : "No") << "\n";
        return info.str();
    }

    void setAllowedCountries(const std::vector<std::string>& countries) {
        allowedCountries = countries;
    }
};