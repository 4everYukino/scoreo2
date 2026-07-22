include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# base implements for httpd
add_library(httpd SHARED
    http_acceptor.cpp
    http_echo_handler.cpp
    http_handler.cpp
    http_handler_factory.cpp
    http_helper.cpp
    http_router.cpp
    http_session.cpp
    http_uri_parser.cpp
)

target_link_libraries(httpd PRIVATE spdlog)

set_target_properties(httpd PROPERTIES
        INSTALL_RPATH "$ORIGIN")

install(TARGETS httpd
        RUNTIME DESTINATION "${PRODUCT_INSTALL_ROOT}" COMPONENT Runtime
        LIBRARY DESTINATION "${PRODUCT_INSTALL_ROOT}" COMPONENT Runtime
        ARCHIVE DESTINATION "${PRODUCT_INSTALL_ROOT}" COMPONENT Runtime)
