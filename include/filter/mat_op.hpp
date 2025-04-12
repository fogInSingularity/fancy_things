#ifndef MAT_OP_HPP_
#define MAT_OP_HPP_

#include <cstdint>
#include <cstddef>

#include <spdlog/spdlog.h>

#include "filter/utility.hpp"
#include "filter/matrix.hpp"

namespace ftr {

template <typename T, typename U>
Matrix<T> Convolution(const Matrix<T>& target, const Matrix<U>& kernel);

template <typename T>
Matrix<T> NormalizeTo1(const Matrix<T>& mat);

template <typename T, typename Func>
Matrix<T> Apply(const Matrix<T>& mat, Func func);

// impl

template <typename T, typename U>
Matrix<T> Convolution(const Matrix<T>& target, const Matrix<U>& kernel) {
    spdlog::debug("Convolution ...");
    int64_t idim_x = target.Width();
    int64_t idim_y = target.Height();

    int64_t kdim_x = kernel.Width();
    int64_t kdim_y = kernel.Height();

    Matrix<T> res_mat{target.GetSize()};

    for (int64_t i = 0; i < idim_x; i++) {
        for (int64_t j = 0; j < idim_y; j++) {
            T sum{};
            for (int64_t k = 0; k < kdim_x; k++) {
                for (int64_t l = 0; l < kdim_y; l++) {
                    int64_t current_dim_x = i + k - (kdim_x / 2);
                    int64_t current_dim_y = j + l - (kdim_y / 2);

                    if (current_dim_y >= 0 && current_dim_y < idim_y && current_dim_x >= 0 && current_dim_x < idim_x) {
                        sum += target.At(current_dim_x, current_dim_y) * kernel.At(k, l);
                    }
                }
            }
            
            // spdlog::trace("[{}, {}] sum value: {}", i, j, sum);
            res_mat.At(i, j) = sum;
        }
    }

    return res_mat;
}

template <typename T>
Matrix<T> NormalizeTo1(const Matrix<T>& mat) {
    spdlog::trace("Matrix norm to 1:");

    Size size = mat.GetSize();
    Matrix<T> norm_mat{size};

    T sum{};

    for (size_t i = 0; i < size.w; i++) {
        for (size_t j = 0; j < size.h; j++) {
            sum += mat.At(i, j);
        }
    }

    for (size_t i = 0; i < size.w; i++) {
        for (size_t j = 0; j < size.h; j++) {
            norm_mat.At(i, j) = mat.At(i, j) / sum;
        }
    }

    return norm_mat;
}

template <typename T, typename Func>
Matrix<T> Apply(const Matrix<T>& mat, Func func) {
    spdlog::trace("Matrix<T>::Apply call");
    Size size = mat.GetSize();

    Matrix<T> new_mat{size};
    for (size_t i = 0; i < size.w; i++) {
        for (size_t j = 0; j < size.h; j++) {
            new_mat.At(i, j) = func(mat.At(i, j));
        }
    }

    return new_mat;
}

} // namespace ftr

#endif // MAT_OP_HPP_
