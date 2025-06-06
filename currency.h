#pragma once
#include <iostream>

class Currency {
protected:
    std::string code;
    std::string symbol;
    std::string type;
    double exchangeRateToBase;
public:
    Currency(const std::string& _code, const std::string& _symbol, 
        const std::string& _type) :
    code(_code), symbol(_symbol), type(_type), exchangeRateToBase(0.0) {}
    
    virtual ~Currency() = default;
    virtual double getRate() const = 0;
    virtual void fluctuate() = 0;
    virtual double applyTaxOrFee(double amount) const = 0;
    virtual std::string makeReport(double amount) const = 0;
    virtual bool isStable() const = 0;
    virtual bool canBeUsedIn(const std::string& country) const = 0;

    std::string getCode() const { return code; }
    std::string getSymbol() const { return symbol; }
    std::string getType() const { return type; }
};