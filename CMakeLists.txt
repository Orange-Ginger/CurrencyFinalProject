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

include_directories(${CMAKE_SOURCE_DIR}/headers)

add_executable(currency_converter
    source/currency.cpp
    source/currency_parser.cpp
    source/text_formatting.cpp
    source/main.cpp
)

enable_testing()
add_subdirectory(test)