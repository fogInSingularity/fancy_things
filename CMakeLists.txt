cmake_minimum_required(VERSION 3.21) # current lts version

project(fancy_things)

add_executable(${PROJECT_NAME})

target_compile_features(${PROJECT_NAME}
    PUBLIC
        cxx_std_20
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/filter/filters.cpp
        src/fcy_ui/fancy_things.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include/
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -fstack-protector-strong
        -fcheck-new
        -fstrict-overflow

        $<$<CONFIG:Debug>:
            -Og
            -g3
            -ggdb
            # -fsanitize=address,leak,undefined
        >

        $<$<CONFIG:Release>:
            -O2
            -march=native
            -flto
            -DNDEBUG
        >
)

target_link_options(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:
            -Og
            -g3
            -ggdb
            # -fsanitize=address,leak,undefined
        >

        $<$<CONFIG:Release>:
            -O2
            -march=native
            -flto
        >
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

# to output colors
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -fdiagnostics-color=always
    )
endif()

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        sfml-graphics 
        sfml-window 
        sfml-system
        spdlog
)

# use cpm package manager
include(cmake/CPM.cmake)

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    # GIT_TAG v2.x
    GIT_TAG v1.15.0
)

enable_testing()
add_subdirectory(tests)
