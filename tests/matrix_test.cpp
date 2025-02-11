#include <gtest/gtest.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <matrix.hpp>

TEST(MatrixTest, MatrixMul) {
    fcy::Matrix<int> mat1x1_a{1, 1, {1}};
    fcy::Matrix<int> mat1x1_b{1, 1, {0}};
    fcy::Matrix<int> mat1x1_c{1, 1, {1}};
    auto res1x1 = mat1x1_a * mat1x1_b;
    spdlog::trace("{}", res1x1);
    EXPECT_EQ(res1x1.GetElem(0, 0), 0);
    res1x1 = mat1x1_a * mat1x1_c;
    spdlog::trace("{}", res1x1);
    EXPECT_EQ(res1x1.GetElem(0, 0), 1);

    fcy::Matrix<int> a{2, 2, {1, 0, 0, 1}};
    fcy::Matrix<int> b{2, 2, {1, 0, 0, 1}};
    auto res = a * b;
    spdlog::trace("{}", res);
    EXPECT_EQ(res.GetElem(0, 0), 1);
    EXPECT_EQ(res.GetElem(1, 0), 0);

    res = a + b;
    spdlog::trace("{}", res);
    EXPECT_EQ(res.GetElem(0, 0), 2);
    EXPECT_EQ(res.GetElem(1, 0), 0);
}

int main(int argc, char** argv) {
    auto logger = spdlog::basic_logger_mt("fancy_things", "fancy_tests.log", true);
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
