#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "currency.h"
#include "currency_parser.h"
#include <windows.h>

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

        if (choice == 1) converter.listAllCurrencies();
        else if (choice == 2) {
            std::string fromCode, toCode, userCountry;
            double amount;

            std::cout << "Enter your country: ";
            std::cin.ignore();
            std::getline(std::cin, userCountry);
            std::cout << "Enter FROM currency code: ";
            std::cin >> fromCode;
            std::cout << "Enter TO currency code: ";
            std::cin >> toCode;
            std::cout << "Enter amount to convert: ";
            std::cin >> amount;

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