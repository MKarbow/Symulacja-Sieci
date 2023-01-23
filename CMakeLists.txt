cmake_minimum_required(VERSION 3.21)
project(Symulacja_Sieci)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Wpedantic -pedantic-errors)

include_directories(
        "include"
)

set(SOURCE_FILES
        "src/package.cpp"
        "src/storage_types.cpp"
        "src/nodes.cpp"
        "src/helpers.cpp"
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES} "main.cpp")

target_compile_definitions(${PROJECT_NAME} PUBLIC EXERCISE_ID=2)

set(SOURCES_FILES_TESTS
        "test/test_package.cpp"
        "test/test_storage_types.cpp"
        "test/test_nodes.cpp"
        )

add_executable("${PROJECT_NAME}_test" ${SOURCE_FILES} ${SOURCES_FILES_TESTS} "test/main_gtest.cpp")

target_include_directories("${PROJECT_NAME}_test" PUBLIC
        "googletest-master/googlemock/include"
        "googletest-master/googletest/include"
        "mocks"
        )

add_subdirectory("googletest-master")

target_link_libraries("${PROJECT_NAME}_test" "gmock")