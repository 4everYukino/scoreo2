project(tests)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(run_tests
    test_main.cpp

    http_uri_parser_test.cpp
)

target_link_libraries(run_tests
    PRIVATE
        gtest
        gtest_main

        httpd
)

enable_testing()
add_test(NAME ALL_TESTS COMMAND run_tests)
