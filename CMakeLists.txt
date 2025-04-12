cmake_minimum_required(VERSION 3.21)

project(fancy_things)

include(cmake/defaults.cmake)
include(cmake/third_party.cmake)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/filter/filters.cpp
        src/fcy_ui/fancy_things.cpp
        src/bridge/thread_bridge.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include/
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        fancy-defaults
        sfml-graphics 
        sfml-window 
        sfml-system
        spdlog
        dl
)

enable_testing()
add_subdirectory(tests)
