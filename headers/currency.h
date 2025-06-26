#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <random>

class Currency {
protected:
    std::string code, symbol, type;
    double rate, taxRate;
public:
    Currency(const std::string& _code, const std::string& _symbol, 
             const std::string& _type, double _rate, double _taxRate);
    Currency(const Currency&) = delete;
    Currency& operator=(const Currency&) = delete;
    virtual ~Currency() = default;
    virtual void fluctuate() = 0;
    virtual double applyTaxOrFee(double amount) const = 0;
    virtual std::string makeReport(double amount, const std::string& country) const = 0;
    virtual bool isStable() const = 0;
    virtual bool canBeUsedIn(const std::string& country) const = 0;
    std::string getCode() const;
    std::string getSymbol() const;
    std::string getType() const;
    double getRate() const;
    double getTaxRate() const;
};

class CryptoCurrency : public Currency {
private:
    double volatility;
    std::vector<std::string> allowedCountries;
public:
    CryptoCurrency(const std::string&, const std::string&, double, double, double, const std::vector<std::string>&);
    CryptoCurrency(const CryptoCurrency&) = delete;
    CryptoCurrency& operator=(const CryptoCurrency&) = delete;
    void fluctuate() override;
    double applyTaxOrFee(double amount) const override;
    std::string makeReport(double amount, const std::string& country) const override;
    bool isStable() const override;
    bool canBeUsedIn(const std::string& country) const override;
    double getVolatility() const;
};

class FiatCurrency : public Currency {
private:
    double inflationRate;
    std::vector<std::string> allowedCountries;
public:
    FiatCurrency(const std::string&, const std::string&, double, double, double, const std::vector<std::string>&);
    FiatCurrency(const FiatCurrency&) = delete;
    FiatCurrency& operator=(const FiatCurrency&) = delete;
    void fluctuate() override;
    double applyTaxOrFee(double amount) const override;
    std::string makeReport(double amount, const std::string& country) const override;
    bool isStable() const override;
    bool canBeUsedIn(const std::string& country) const override;
    double getInflationRate() const;
};

class MagicCurrency : public Currency {
protected:
    int rarityLevel;
    std::string incantation, realmOrigin;
    static inline std::random_device rd;
    static inline std::mt19937 gen;
public:
    MagicCurrency(const std::string&, const std::string&, double, double, int, const std::string&, const std::string&);
    MagicCurrency(const MagicCurrency&) = delete;
    MagicCurrency& operator=(const MagicCurrency&) = delete;
    void fluctuate() override;
    double applyTaxOrFee(double amount) const override;
    std::string makeReport(double amount, const std::string& country) const override;
    bool isStable() const override;
    bool canBeUsedIn(const std::string& country) const override;
    int getRarityLevel() const;
    std::string getIncantation() const;
    std::string getRealmOrigin() const;
};

class CurrencyConverter {
private:
    std::unordered_map<std::string, std::shared_ptr<Currency>> currencies;
public:
    CurrencyConverter();
    CurrencyConverter(const CurrencyConverter&) = delete;
    CurrencyConverter& operator=(const CurrencyConverter&) = delete;
    void addCurrency(const std::shared_ptr<Currency>& currency);
    std::shared_ptr<Currency> getCurrency(const std::string& code) const;
    double convert(const std::string& fromCode, const std::string& toCode, double amount) const;
    std::string getReport(const std::string& code, double amount, const std::string& country) const;
    void fluctuateAll();
    void listAllCurrencies() const;
};