cmake_minimum_required(VERSION 3.17)
project(Containers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

##############################################################################
# Dependencies
##############################################################################
## GTest
add_subdirectory(import/googletest-release-1.10.0)

##############################################################################
# Sources
##############################################################################
## Build services
add_subdirectory(src)

## Build tests
enable_testing()
add_subdirectory(tests)