project(Project)

add_library(soko-lib SHARED main.cpp)

target_link_libraries(soko-lib
    log
)