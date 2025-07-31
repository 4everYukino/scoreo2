CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(scoreo2)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS ON)

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_subdirectory(server)

# basic modules {
OPTION(ENABLE_HTTPD "Enable httpd module" ON)

if (ENABLE_HTTPD)
    add_subdirectory(httpd)
endif()
# }

# business modules {
OPTION(ENABLE_SCORING "Enable scoring module" ON)

if (ENABLE_SCORING)
    add_subdirectory(scoring)
endif()
# }

# third-party modules {
add_subdirectory(src/fmt)

add_subdirectory(src/json)

set(SPDLOG_INSTALL ON)
set(SPDLOG_BUILD_SHARED ON)
set(SPDLOG_BUILD_PIC TRUE)
add_subdirectory(src/spdlog)
# }

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(UNIX)
  set(OPT_PREFIX /opt/${PROJECT_NAME})

  set(PRODUCT_LOG_PREFIX /var/log/${PROJECT_NAME})

  set(SYSTEMD_UNIT_DIR "/lib/systemd/system")
endif()

configure_file(
  ${CMAKE_SOURCE_DIR}/config.h.in
  ${CMAKE_BINARY_DIR}/config.h
  @ONLY
)

configure_file(
  ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.service.in
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.service
  @ONLY
)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.service"
  DESTINATION "${SYSTEMD_UNIT_DIR}"
)
