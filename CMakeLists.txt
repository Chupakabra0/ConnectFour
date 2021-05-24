project(RealConnectFour)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

find_program(NUGET_EXE NAMES nuget)
if(NOT NUGET_EXE)
    message(FATAL_ERROR
            "NuGet hasn't been found. "
            "Please, install this executable, and run CMake again\n")
endif()

set(NUGET_PACKAGES_DIR ${CMAKE_SOURCE_DIR}/packages)

exec_program("${NUGET_EXE} install fmt -Version 7.0.1 -OutputDirectory \"${NUGET_PACKAGES_DIR}\"")

include_directories(include "${NUGET_PACKAGES_DIR}/fmt.7.0.1/include")

add_executable(RealConnectFour RealConnectFour/Main.cpp)
