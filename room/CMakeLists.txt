project(room)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(room SHARED
    pool_based_room.cpp

    room_manager.cpp
)

target_link_libraries(room PRIVATE utils)

install(TARGETS room
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
