# use cpm package manager
include(cmake/CPM.cmake)

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    # GIT_TAG v2.x
    GIT_TAG v1.15.0
    OPTIONS "SPDLOG_BUILD_PIC ON"
)

CPMAddPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG v1.16.0
    OPTIONS "INSTALL_GTEST OFF"
)
