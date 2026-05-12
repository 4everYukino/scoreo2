project(process)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(process SHARED
    process_handler.cpp
    process_killer.cpp
    process_manager.cpp

    aps_finished_storage.cpp
    aps_p90_estimator.cpp
    aps_pending_queue.cpp
    aps_scheduler.cpp
)

target_link_libraries(process PRIVATE spdlog)

install(TARGETS process
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

OPTION(ENABLE_APS_SOLVER "Enable to build APS Solver" OFF)

if (ENABLE_APS_SOLVER)
    add_executable(aps_solver
        aps_solver.cpp
    )

    install(TARGETS aps_solver
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif()
