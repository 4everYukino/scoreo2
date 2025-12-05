project(process)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(process SHARED
    process_handler.cpp
    process_killer.cpp
    process_manager.cpp
)

target_link_libraries(process PRIVATE spdlog)

install(TARGETS process
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
