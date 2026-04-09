project(qtlab)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(qtlab SHARED
    repo/daily_bar_repo.cpp

    tushare/tushare_client.cpp
    tushare/tushare_client_impl.cpp
)

target_link_libraries(qtlab
    PUBLIC
        nlohmann_json
    PRIVATE
        curl_client
        spdlog)

install(TARGETS qtlab
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
