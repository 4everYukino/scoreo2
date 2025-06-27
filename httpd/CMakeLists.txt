project(httpd)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# base implements for httpd
add_library(httpd SHARED
    http_handler.cpp
    http_router.cpp
    http_session.cpp
)

# first business: scoreo {
target_sources(httpd PRIVATE
    http_room_handler.cpp
)

target_link_libraries(httpd PRIVATE fmt nlohmann_json room)

# }

install(TARGETS httpd
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
