#include "text_formatting.h"
#include <iostream>
#include <limits>
#include <algorithm>

std::string normalizeCode(const std::string& code) {
    size_t start = code.find_first_not_of(" \t");
    size_t end = code.find_last_not_of(" \t");
    std::string trimmed = (start == std::string::npos) ? "" : code.substr(start, end - start + 1);

    std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return trimmed;
}

std::string normalizeCountry(const std::string& country) {
    size_t start = country.find_first_not_of(" \t");
    size_t end = country.find_last_not_of(" \t");
    std::string trimmed = (start == std::string::npos) ? "" : country.substr(start, end - start + 1);

    std::string collapsed;
    bool inSpace = false;
    for (char c : trimmed) {
        if (c == ' ' || c == '\t') {
            if (!inSpace) {
                collapsed += ' ';
                inSpace = true;
            }
        } else {
            collapsed += c;
            inSpace = false;
        }
    }

    bool newWord = true;
    std::transform(collapsed.begin(), collapsed.end(), collapsed.begin(),
                   [&newWord](unsigned char c) -> unsigned char {
                        if (newWord) {
                            newWord = false;
                            return std::toupper(c);
                        }
                        if (c == ' ' || c == '-' || c == '\'' && !newWord) {
                            newWord = true;
                            return c;
                        }
                        return std::tolower(c);
                    });
    return collapsed;
}

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