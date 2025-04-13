#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <cassert>
#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <stdexcept>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ranges.h> // for initializer_list

#include "helpers/traceable_exception.hpp"
#include "filter/utility.hpp"

namespace ftr {

template <typename T>
class Matrix {
  private:
    Size size_;
    std::vector<T> mat_memory_;

    size_t InternalIndex(size_t i, size_t j) const {
        return j * size_.w + i;
    }
  public:
    explicit Matrix(Size size, const T* matrix_array = nullptr);
    Matrix(Size size, const std::initializer_list<T>& matrix_list);
    Matrix& operator=(std::initializer_list<T> matrix_list);
    
    size_t Width()  const { return size_.w; };
    size_t Height() const { return size_.h; };
    Size GetSize()  const { return size_; }

    T& At(size_t i, size_t j) {
        if ((i >= size_.w) || (j >= size_.h)) {
            spdlog::error("matrix get elem out of rang: i:{}, j:{}, dim x:{}, dim y:{}", i, j, size_.w, size_.h);
            throw std::out_of_range{"matrix At out of range"};
        }

        return mat_memory_[InternalIndex(i, j)];
    }

    const T& At(size_t i, size_t j) const {
        if ((i >= size_.w) || (j >= size_.h)) {
            spdlog::error("matrix get elem out of rang: i:{}, j:{}, dim x:{}, dim y:{}", i, j, size_.w, size_.h);
            throw std::out_of_range{"matrix At out of range"};
        }

        return mat_memory_[InternalIndex(i, j)];
    }

    const T* GetData() const {
        return mat_memory_.data();
    }
};

template<typename T>
Matrix<T> operator*(const Matrix<T>& matrix_a, const Matrix<T>& matrix_b);

template<typename T>
Matrix<T> operator+(const Matrix<T>& matrix_a, const Matrix<T>& matrix_b);

// impl

template <typename T>
Matrix<T>::Matrix(Size size, const T* matrix_array) 
    : size_{size}, mat_memory_{}
{
    spdlog::trace("Matrix constructor call: {:p} {}x{}", reinterpret_cast<const void*>(matrix_array), size_.w, size_.h);

    size_t area = size_.w * size_.h;
    mat_memory_.resize(area);

    spdlog::trace("area: {}", area);
    if (matrix_array != nullptr) {
        mat_memory_.assign(matrix_array, matrix_array + area);
    }
    spdlog::trace("vec size: {}", mat_memory_.size());
}

template <typename T>
Matrix<T>::Matrix(Size size, const std::initializer_list<T>& matrix_list) 
    : size_{size}, mat_memory_{}
{
    spdlog::trace("Matrix constructor call: {} {}x{}", matrix_list, size_.w, size_.h);

    if (matrix_list.size() > size.w * size.h) {
        spdlog::error("size of std::initializer_list(= {}) bigger than size of matrix(= {}x{})", matrix_list.size(), size.w, size.h);
        throw hlp::TraceableException("size of std::initializer_list bigger than size of matrix");
    }

    size_t area = size_.w * size_.h;
    mat_memory_.resize(area);

    mat_memory_.assign(matrix_list.begin(), matrix_list.end());
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(std::initializer_list<T> mat_list) {
    mat_memory_.assign(mat_list.begin(), mat_list.end());

    return *this;
}

//    n            p         p
// m      *   n        = m 
// 
template <typename T>
Matrix<T> operator*(const Matrix<T>& matrix_a, const Matrix<T>& matrix_b) {
    if (matrix_a.Width() != matrix_b.Height()) { 
        spdlog::error("invalid matrix sizes for multiplication"); 
        throw std::invalid_argument{"matrix operator* invalid dimentions passed"};
    }

    Matrix<T> matrix_c = Matrix<T>{{matrix_b.Width(), matrix_a.Height()}};

    size_t dim_n = matrix_a.Width();
    size_t dim_m = matrix_a.Height();
    size_t dim_p = matrix_b.Width();

    for (size_t j = 0; j < dim_m; j++) {
        for (size_t i = 0; i < dim_p; i++) {
            // inner loop
            for (size_t index_sum = 0; index_sum < dim_n; index_sum++) {
                matrix_c.At(i, j) = matrix_c.At(i, j)
                                    + matrix_a.At(index_sum, j) 
                                      * matrix_b.At(i, index_sum);
            }
        }
    }

    return matrix_c;
}

template<typename T>
Matrix<T> operator+(const Matrix<T>& matrix_a, const Matrix<T>& matrix_b) {
    spdlog::trace("Matrix<T>operator+ call");
    if ((matrix_a.Width() != matrix_b.Width()) || (matrix_a.Height() != matrix_b.Height())) {
        spdlog::error("invalid matrix sizes for multiplication"); 
        throw std::invalid_argument{"matrix operator+ invalid dimentions passed"};
    }

    size_t dim_x = matrix_a.Width();
    size_t dim_y = matrix_b.Height();

    Matrix<T> matrix_c = Matrix<T>{{dim_x, dim_y}};
    for (size_t i = 0; i < dim_x; i++) {
        for (size_t j = 0; j < dim_y; j++) {
            matrix_c.At(i, j) = matrix_a.At(i, j) + matrix_b.At(i, j);
        }
    }

    return matrix_c;
}
} // namespace ftr

// format for matrix

template <typename T>
struct fmt::formatter<ftr::Matrix<T>> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const ftr::Matrix<T>& matrix, FormatContext& ctx) const -> decltype(ctx.out()) {
        auto out = ctx.out();
        size_t dim_x = matrix.Width();
        size_t dim_y = matrix.Height();

        fmt::format_to(out, "Matrix ({}x{}):\n", dim_x, dim_y);

        for (size_t j = 0; j < dim_y; ++j) {
            fmt::format_to(out, "[ ");
            for (size_t i = 0; i < dim_x; ++i) {
                fmt::format_to(out, "{} ", matrix.At(i, j));
            }
            fmt::format_to(out, "]\n");
        }
        return out;
    }
};

#endif // MATRIX_HPP_
