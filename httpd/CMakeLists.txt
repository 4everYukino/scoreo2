project(httpd)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# base implements for httpd
add_library(httpd SHARED
    http_acceptor.cpp
    http_handler.cpp
    http_handler_factory.cpp
    http_helper.cpp
    http_router.cpp
    http_session.cpp
    http_uri_parser.cpp
)

target_link_libraries(httpd PRIVATE spdlog)

install(TARGETS httpd
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
