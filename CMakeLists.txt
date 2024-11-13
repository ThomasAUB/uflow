cmake_minimum_required(VERSION 3.22)            # CMake version check
project(uflow VERSION 0.0.1 LANGUAGES CXX)      # Create project
set(CMAKE_CXX_STANDARD 17)                      # Enable c++17 standard

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/ulink/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})


# if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    enable_testing()

    add_subdirectory(tests)

    # add_test(ustream_tests  ull_tests)
    
    # add_test(NAME ustreamTests COMMAND $<TARGET_FILE:tests/tests.cpp>)
    
# endif()