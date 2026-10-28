cmake_minimum_required(VERSION 3.20)
project(mystl LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(mystl INTERFACE)

target_include_directories(mystl INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(mystl INTERFACE
    -Wall -Wextra -Wconversion
    $<$<CONFIG:Debug>:-g -fsanitize=address,undefined>)

target_link_options(mystl INTERFACE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.9.1)
FetchContent_MakeAvailable(Catch2)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(tests/uniqueptr_)
add_subdirectory(tests/string_)
add_subdirectory(tests/vector_)
