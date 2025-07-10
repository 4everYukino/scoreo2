project(starter)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(starter
        main.cpp

        init_spdlog.cpp)

find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(starter PRIVATE httpd Boost::program_options spdlog)

install(TARGETS starter
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
