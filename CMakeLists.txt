CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(scoreo)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS ON)

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
)

add_subdirectory(server)
add_subdirectory(utils)

# basic modules {
OPTION(ENABLE_HTTPD "Enable httpd module" ON)

if (ENABLE_HTTPD)
    add_subdirectory(httpd)
endif()
# }

# business modules {
OPTION(ENABLE_ROOM "Enable room module" ON)

if (ENABLE_ROOM)
    add_subdirectory(room)
endif()
# }

# third-party modules {
add_subdirectory(src/fmt)
add_subdirectory(src/json)
add_subdirectory(src/spdlog)
# }

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
