cmake_minimum_required(VERSION 3.10)

project(CurrencyProjectTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(currency_tests
    currency_test.cpp
    ../currency_parser.cpp
)

target_link_libraries(currency_tests gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(currency_tests)