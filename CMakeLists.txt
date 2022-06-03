cmake_minimum_required(VERSION 3.10)

project(test-timeserver
    VERSION
    1.0.0
    DESCRIPTION
    "single-threaded app that send UTC time and messages to any TCPv4 client"
    LANGUAGES
    CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB sources src/*.cpp)
add_executable(${PROJECT_NAME} ${sources})

# Build tests
add_subdirectory(tests)
