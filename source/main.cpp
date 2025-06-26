#define NOMINMAX
#include <iostream>
#include <string>
#include "currency.h"
#include "currency_parser.h"
#include "text_formatting.h"
#include <windows.h>
#include <limits>
#include <iomanip>

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
        while (true) {
            std::cin >> choice;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please, enter a real number: ";
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }

        if (choice == 1) converter.listAllCurrencies();
        else if (choice == 2) {
            std::string fromCode, toCode, userCountry;
            double amount;

            userCountry = readValidatedString("Enter your location: ", isString);
            fromCode = readValidatedString("Enter FROM currency code: ", isString);
            toCode = readValidatedString("Enter TO currency code: ", isString);
            amount = readPositiveDouble("Enter amount to convert: ");

            try {
                auto fromCurrency = converter.getCurrency(normalizeCode(fromCode));
                auto toCurrency = converter.getCurrency(normalizeCode(toCode));

                double converted = converter.convert(fromCode, toCode, amount);

                std::cout << "\n===== Conversion Result =====\n";
                std::cout << std::fixed << std::setprecision(3) << amount << " " << fromCurrency->getCode()
                          << " = " << converted << " " << toCurrency->getCode() << "\n";

                std::cout << "\n" << fromCurrency->makeReport(amount, normalizeCountry(userCountry));
                std::cout << "\n" << toCurrency->makeReport(converted, normalizeCountry(userCountry));
            } catch (const std::exception& exception) {
                std::cerr << "Error: " << exception.what() << "\n";
            }
        }
        else if (choice != 3) std::cout << "Invalid operation. Try again.\n";
    } while (choice != 3);

    std::cout << "Thanks for using the converter!\n";

    return 0;
}