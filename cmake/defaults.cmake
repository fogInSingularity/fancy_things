set(TARGET fancy-defaults)

add_library(${TARGET} INTERFACE)

target_compile_features(${TARGET}
    INTERFACE
        cxx_std_20
)

target_compile_options(${TARGET}
    INTERFACE
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

target_link_options(${TARGET}
    INTERFACE
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
    target_compile_options(${TARGET}
        INTERFACE
            -fdiagnostics-color=always
    )
endif()


