#include <exception>

#include <gtest/gtest.h>

#include <spdlog/spdlog.h>

#include "filter/matrix.hpp"

TEST(MatrixTest, MatrixMul) {
    ftr::Matrix<int> mat1x1_a{{1, 1}, {1}};
    ftr::Matrix<int> mat1x1_b{{1, 1}, {0}};
    ftr::Matrix<int> mat1x1_c{{1, 1}, {1}};
    auto res1x1 = mat1x1_a * mat1x1_b;
    spdlog::trace("{}", res1x1);
    EXPECT_EQ(res1x1.At(0, 0), 0);
    res1x1 = mat1x1_a * mat1x1_c;
    spdlog::trace("{}", res1x1);
    EXPECT_EQ(res1x1.At(0, 0), 1);

    ftr::Matrix<int> mat2x2_a{{2, 2}, {1, 0, 0, 1}};
    ftr::Matrix<int> mat2x2_b{{2, 2}, {1, 0, 0, 1}};
    auto res2x2 = mat2x2_a * mat2x2_b;
    spdlog::trace("{}", res2x2);
    EXPECT_EQ(res2x2.At(0, 0), 1);
    EXPECT_EQ(res2x2.At(1, 0), 0);
}

TEST(MatrixTest, MatrixAdd) {
    ftr::Matrix<int> mat2x2_a{{2, 2}, {1, 2, 3, 4}};
    ftr::Matrix<int> mat2x2_b{{2, 2}, {5, 6, 7, 8}};
    auto res = mat2x2_a + mat2x2_b;
    spdlog::trace("{}", res);
    EXPECT_EQ(res.At(0, 0), 6);
    EXPECT_EQ(res.At(1, 1), 12);
}

TEST(MatrixTest, MatrixEx) {
    ftr::Matrix<int> mat{{1, 1}, {1}};
    EXPECT_THROW(mat.At(1, 1), std::exception);
}

