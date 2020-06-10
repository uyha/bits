cmake_minimum_required(VERSION 3.8)
project("river bits"
        VERSION 0.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-fconcepts)
endif ()

add_subdirectory(tests)
target_include_directories(bits_test PRIVATE include)

add_executable(main "main.cpp")
target_compile_features(main PRIVATE cxx_std_20)
