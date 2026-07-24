cmake_minimum_required(VERSION 3.15)

# The product name is defined here and nowhere else in the build system.
# It can also be overridden with -DPRODUCT_NAME=<name> on a fresh configure.
set(PRODUCT_NAME "inks" CACHE STRING "Product name")

if(NOT PRODUCT_NAME MATCHES "^[a-z0-9][a-z0-9+.-]+$")
    message(FATAL_ERROR
        "PRODUCT_NAME must be a valid Debian package name: lowercase letters, "
        "digits, '+', '-' and '.', with at least two characters")
endif()

project(${PRODUCT_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

option(BUILD_TESTING "Build the test suite" OFF)
include(CTest)

option(ENABLE_CURL_CLIENT "Enable cURL client module" ON)
option(ENABLE_HTTPD "Enable httpd module" ON)
option(ENABLE_PROCESS "Enable process module" ON)
option(ENABLE_APS_SOLVER "Enable APS solver executable" OFF)

if(NOT UNIX)
    message(FATAL_ERROR "${PRODUCT_NAME} currently supports UNIX systems only")
endif()

set(PRODUCT_INSTALL_ROOT "usr/lib/${PRODUCT_NAME}")
set(PRODUCT_BIN_DIR "/${PRODUCT_INSTALL_ROOT}")
set(PRODUCT_LIB_DIR "/${PRODUCT_INSTALL_ROOT}")
set(PRODUCT_ETC_ROOT "/etc/${PRODUCT_NAME}")
set(PRODUCT_CONF_DIR "${PRODUCT_ETC_ROOT}/conf")
set(PRODUCT_LOG_DIR "/var/log/${PRODUCT_NAME}")
set(PRODUCT_SYSTEMD_UNIT_DIR "usr/lib/systemd/system")

configure_file(
    "${CMAKE_SOURCE_DIR}/config.h.in"
    "${CMAKE_BINARY_DIR}/config.h"
    @ONLY
)

configure_file(
    "${CMAKE_SOURCE_DIR}/product.service.in"
    "${CMAKE_BINARY_DIR}/${PRODUCT_NAME}.service"
    @ONLY
)

include_directories(
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_SOURCE_DIR}"
)

# Dependencies are embedded for building, but only required runtime artifacts
# are installed into the product package.
set(JSON_Install OFF CACHE BOOL "" FORCE)
add_subdirectory(src/json)

set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_SHARED ON CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_PIC ON CACHE BOOL "" FORCE)
add_subdirectory(src/spdlog)

if(ENABLE_HTTPD)
    add_subdirectory(httpd)
endif()

if(ENABLE_CURL_CLIENT)
    add_subdirectory(curl_client)
endif()

if(ENABLE_PROCESS)
    add_subdirectory(process)
endif()

if(NOT ENABLE_HTTPD)
    message(FATAL_ERROR "ENABLE_HTTPD must be ON to build ${PRODUCT_NAME}")
endif()
add_subdirectory(server)

if(BUILD_TESTING)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    add_subdirectory(src/googletest)
    add_subdirectory(tests)
endif()

install(
    TARGETS spdlog
    RUNTIME DESTINATION "${PRODUCT_INSTALL_ROOT}" COMPONENT Runtime
    LIBRARY DESTINATION "${PRODUCT_INSTALL_ROOT}" COMPONENT Runtime
    ARCHIVE DESTINATION "${PRODUCT_INSTALL_ROOT}" COMPONENT Runtime
)

set_target_properties(spdlog PROPERTIES INSTALL_RPATH "$ORIGIN")

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT_NAME}.service"
    DESTINATION "${PRODUCT_SYSTEMD_UNIT_DIR}"
    COMPONENT Systemd
)

install(
    FILES "${CMAKE_SOURCE_DIR}/conf/product.conf"
    DESTINATION "etc/${PRODUCT_NAME}/conf"
    RENAME "${PRODUCT_NAME}.conf"
    COMPONENT Configuration
)

install(
    FILES "${CMAKE_SOURCE_DIR}/conf/handler.conf"
    DESTINATION "etc/${PRODUCT_NAME}/conf"
    COMPONENT Configuration
)

set(DEBIAN_CONTROL_DIR "${CMAKE_BINARY_DIR}/debian")
file(MAKE_DIRECTORY "${DEBIAN_CONTROL_DIR}")

foreach(control_file IN ITEMS conffiles postinst prerm postrm)
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/debian/${control_file}.in"
        "${DEBIAN_CONTROL_DIR}/${control_file}"
        @ONLY
        NEWLINE_STYLE UNIX
    )
endforeach()

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "${PRODUCT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Lightweight HTTP service")
set(CPACK_PACKAGE_CONTACT "Bruce <sfboy4ever@163.com>"
    CACHE STRING "Debian package maintainer")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/packages")
set(CPACK_PACKAGING_INSTALL_PREFIX "/")

set(CPACK_DEBIAN_PACKAGE_NAME "${PRODUCT_NAME}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
set(CPACK_DEBIAN_PACKAGE_SECTION "web")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_RELEASE "1")
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/4everYukino/inks")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "init-system-helpers (>= 1.18~)")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION ON)
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    "${DEBIAN_CONTROL_DIR}/conffiles"
    "${DEBIAN_CONTROL_DIR}/postinst"
    "${DEBIAN_CONTROL_DIR}/prerm"
    "${DEBIAN_CONTROL_DIR}/postrm"
)

include(CPack)
