project(scoring)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(scoring SHARED
    pool_based_room.cpp

    room_manager.cpp

    utils/game.cpp
)

target_link_libraries(scoring PRIVATE spdlog)

install(TARGETS scoring
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
