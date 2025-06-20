#define NOMINMAX
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "currency.h"
#include "currency_parser.h"
#include <windows.h>
#include <limits>
#include <cctype>

bool isString(const std::string& s) {
    for (char c : s)
        if (!std::isalpha(c) && c != ' ' && c != '-' && c != '\'') return false;
    return !s.empty();
}

std::string readValidatedString(const std::string& prompt, bool (*validator)(const std::string&)) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (validator(input)) return input;
        std::cout << "Invalid input. Please try again.\n";
    }
}

double readPositiveDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (!std::cin.fail() && value > 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a positive number.\n";
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    CurrencyConverter converter;

    auto cryptoCurrencies = loadCryptoCurrencies("crypto_exchange_rates.csv");
    auto fiatCurrencies = loadFiatCurrencies("fiat_exchange_rates.csv");
    auto magicCurrencies = loadMagicCurrencies("magic_exchange_rates.csv");

    for (const auto& c : cryptoCurrencies) converter.addCurrency(c);
    for (const auto& c : fiatCurrencies) converter.addCurrency(c);
    for (const auto& c : magicCurrencies) converter.addCurrency(c);

    int choice;
    do {
        std::cout << "\n===== Currency Converter =====\n";
        std::cout << "1. List all currencies\n";
        std::cout << "2. Convert between currencies\n";
        std::cout << "3. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) converter.listAllCurrencies();
        else if (choice == 2) {
            std::string fromCode, toCode, userCountry;
            double amount;

            userCountry = readValidatedString("Enter your location: ", isString);
            fromCode = readValidatedString("Enter FROM currency code: ", isString);
            toCode = readValidatedString("Enter TO currency code: ", isString);
            amount = readPositiveDouble("Enter amount to convert: ");

            try {
                auto fromCurrency = converter.getCurrency(fromCode);
                auto toCurrency = converter.getCurrency(toCode);

                double converted = converter.convert(fromCode, toCode, amount);

                std::cout << "\n===== Conversion Result =====\n";
                std::cout << amount << " " << fromCurrency->getCode()
                          << " = " << converted << " " << toCurrency->getCode() << "\n";

                std::cout << "\n" << fromCurrency->makeReport(amount, userCountry);
                std::cout << "\n" << toCurrency->makeReport(converted, userCountry);
            } catch (const std::exception& exception) {
                std::cerr << "Error: " << exception.what() << "\n";
            }
        }
        else if (choice != 3) std::cout << "Invalid operation. Try again.\n";
    } while (choice != 3);

    std::cout << "Thanks for using the converter!\n";

    return 0;
}