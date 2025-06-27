project(starter)

add_executable(starter main.cpp)

install(TARGETS starter
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
