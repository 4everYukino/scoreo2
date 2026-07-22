include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${PRODUCT_NAME}
        main.cpp

        initializer.cpp)

target_link_libraries(${PRODUCT_NAME} PRIVATE httpd nlohmann_json spdlog)

set_target_properties(${PRODUCT_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN")

install(TARGETS ${PRODUCT_NAME}
        RUNTIME DESTINATION "${PRODUCT_INSTALL_ROOT}" COMPONENT Runtime
        LIBRARY DESTINATION "${PRODUCT_INSTALL_ROOT}" COMPONENT Runtime
        ARCHIVE DESTINATION "${PRODUCT_INSTALL_ROOT}" COMPONENT Runtime)
