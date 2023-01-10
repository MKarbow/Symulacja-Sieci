cmake_minimum_required(VERSION 3.21)
project(Symulacja_Sieci)

set(CMAKE_CXX_STANDARD 17)

add_executable(Symulacja_Sieci main.cpp)


set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        include/package.hpp
        include/storage_types.hpp
        include/types.hpp
        include/config.hpp
        )

set(SOURCE_FILES_TESTS
        test/test_package.cpp
        test/test_storage_types.cpp
        )



add_subdirectory(googletest-master)