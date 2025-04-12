#include <gtest/gtest.h>

#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>

int main(int argc, char** argv) {
    auto logger = spdlog::basic_logger_mt("fancy_things", "tests.log", true);
    spdlog::set_default_logger(logger);

#if defined (NDEBUG)
    spdlog::set_level(spdlog::level::info);
#else // NDEBUG
    // spdlog::flush_on(spdlog::level::trace);
    spdlog::set_level(spdlog::level::trace);
#endif // NDEBUG 

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
