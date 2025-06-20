cmake_minimum_required(VERSION 4.0.1)

project(CurrencyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(currency_converter
    currency.cpp
    currency_parser.cpp
)

enable_testing()
add_subdirectory(test)