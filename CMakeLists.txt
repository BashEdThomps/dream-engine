cmake_minimum_required (VERSION 2.8.0)
project (dream)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

set(CMAKE_BUILD_TYPE Debug)

# Dream
add_subdirectory (src)

# Dream Unit Tests
add_subdirectory (test)
# Enable Unit Testing
enable_testing()
add_test(NAME DreamTest COMMAND test/DreamTest)
