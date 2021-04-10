cmake_minimum_required(VERSION 3.19)
project(ConnectFour)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_compile_definitions(DEBUG)
endif()

add_executable(ConnectFour
        src/main.cpp include/Board/Board.hpp)
