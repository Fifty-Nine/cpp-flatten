project(cpp-flatten)

cmake_minimum_required(VERSION 3.0)
install(
    FILES flatten/flatten.hpp 
    DESTINATION include/flatten
)

find_package(Boost
    1.36.0
    COMPONENTS unit_test_framework
)

if(${Boost_FOUND})
    add_subdirectory(tests)
else(${Boost_FOUND})
    message(STATUS "Boost.Test not found, unit tests will not be built.")
endif(${Boost_FOUND})
