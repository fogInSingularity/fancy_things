add_executable(${PROJECT_NAME}_tests) 

target_sources(${PROJECT_NAME}_tests
    PRIVATE 
        main_test.cpp
        matrix_test.cpp
)

target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        GTest::gtest_main
        spdlog
)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)

CPMAddPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG v1.16.0
    OPTIONS "INSTALL_GTEST OFF"
)
