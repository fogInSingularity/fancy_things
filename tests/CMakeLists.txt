set(TEST
    ${PROJECT_NAME}_tests
)

if(googletest_ADDED)   
    add_executable(${TEST}
        main_test.cpp
        matrix_test.cpp
    )
    
    target_link_libraries(${TEST}
        PRIVATE GTest::gtest_main
    )

    target_link_libraries(${TEST} PRIVATE spdlog)
    
    target_include_directories(${TEST}
        PRIVATE
            ${CMAKE_SOURCE_DIR}/include
    )

    add_test(NAME ${TEST} COMMAND ${TEST})
endif()
