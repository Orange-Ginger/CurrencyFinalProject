cmake_minimum_required(VERSION 4.0.1)

project(CurrencyProjectTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/headers)

add_executable(currency_tests
    currency_test.cpp
    ../source/currency_parser.cpp
    ../source/currency.cpp
    ../source/text_formatting.cpp
)

target_link_libraries(currency_tests gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(currency_tests)