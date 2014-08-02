add_executable(tests
    tests.cpp
)

target_link_libraries(tests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

target_include_directories(
    tests 
    PUBLIC ${cpp-flatten_SOURCE_DIR} ${Boost_INCLUDE_DIRS}
)

add_custom_target(check
    COMMAND tests
)
