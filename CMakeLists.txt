cmake_minimum_required(VERSION 3.21)
project(Symulacja_Sieci)

set(CMAKE_CXX_STANDARD 17)

add_executable(Symulacja_Sieci main.cpp mocks/nodes_mocks.hpp mocks/global_functions_mock.hpp src/helpers.cpp src/nodes.cpp src/package.cpp src/storage_types.cpp include/config.hpp include/types.hpp include/package.hpp include/storage_types.hpp include/helpers.hpp include/nodes.hpp)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(include)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        )

add_executable(Sieci main.cpp ${SOURCE_FILES})

set(SOURCE_FILES_TESTS
        test/test_package.cpp
        test/test_storage_types.cpp
        test/test_nodes.cpp
        )

add_executable(Sieci_test ${SOURCE_FILES} ${SOURCE_FILES_TESTS} test/main_gtest.cpp)

target_include_directories(Sieci_test PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )

add_subdirectory(googletest-master)

target_link_libraries(Sieci_test gmock)

target_compile_definitions(Sieci_test PUBLIC EXERCISE_ID=2)