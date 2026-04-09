project(curl_client)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(curl_client SHARED
    curl_client.cpp
)

find_package(CURL REQUIRED)

target_link_libraries(curl_client
    PUBLIC
        CURL::libcurl
    PRIVATE
        spdlog)

install(TARGETS curl_client
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
