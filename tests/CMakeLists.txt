cmake_minimum_required(VERSION 3.10)

project(test-timeserver-tests
    VERSION
    1.0.0
    DESCRIPTION
    "Launch unit tests for test-timeserver"
    LANGUAGES
    CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(source_files
    "../src/client.cpp"
    "../src/server.cpp"
    "../src/validator.cpp"
)

file(GLOB test_files *.cpp)

add_executable(${PROJECT_NAME} ${source_files} ${test_files})
target_include_directories(${PROJECT_NAME} PRIVATE ../src)
target_link_libraries(${PROJECT_NAME} PRIVATE -lpthread)
