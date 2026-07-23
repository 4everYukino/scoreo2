include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(run_tests
    test_main.cpp

    http_router_matching_test.cpp
    http_uri_parser_test.cpp
    path_normalize_test.cpp
)

target_link_libraries(run_tests
    PRIVATE
        gtest
        gtest_main

        httpd
)

include(GoogleTest)
gtest_discover_tests(run_tests)
