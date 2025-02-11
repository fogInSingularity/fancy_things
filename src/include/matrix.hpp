#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <cstddef>
#include <cstring>
#include <algorithm>
#include <initializer_list>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ranges.h> // for initializer_list

#include "fcy_assert.hpp"

namespace fcy {

enum class MatrixException {
    Ok                    = 0,
    UninitEnum            = 1,
    InvalidMultiplication = 2,
    InvalidAddition       = 3,
    OutOfRange            = 4,
};

template <typename T>
class Matrix {
  private:
    size_t dim_x_;
    size_t dim_y_;
    T* mat_memory_;

    size_t InternalIndex(size_t i, size_t j) const {
        return j * dim_x_ + i;
    }
  public:
    Matrix(const size_t dim_x, const size_t dim_y, const T* matrix_array = nullptr);
    Matrix(const size_t dim_x, const size_t dim_y, const std::initializer_list<T>& matrix_list);
    Matrix(const Matrix& mat);
    template <typename U>
    explicit Matrix(const Matrix<U>& mat);
    Matrix& operator=(const Matrix& mat);
    Matrix& operator=(std::initializer_list<T> matrix_list);
    template <typename U>
    Matrix& operator=(const Matrix<U>& mat); 

    ~Matrix() {
        delete[] mat_memory_;
    }

    size_t GetDimX() const { return dim_x_; };
    size_t GetDimY() const { return dim_y_; };

    T GetElem(size_t index_i, size_t index_j) const { 
        if ((index_i > dim_x_) || (index_j > dim_y_)) {
            spdlog::error("matrix get elem out of rang: i:{}, j:{}, dim x:{}, dim y:{}", index_i, index_j, dim_x_, dim_y_);
            throw MatrixException::OutOfRange;
        }

        return mat_memory_[InternalIndex(index_i, index_j)];
    }

    void SetElem(size_t index_i, size_t index_j, T elem) {
        if ((index_i > dim_x_) || (index_j > dim_y_)) {
            spdlog::error("matrix set elem out of range: i:{}, j:{}, dim x:{}, dim y:{}", index_i, index_j, dim_x_, dim_y_);
            throw MatrixException::OutOfRange;
        }

        mat_memory_[InternalIndex(index_i, index_j)] = elem;
    }

    void SetElemsFromMem(const T* mem) {
        std::copy(mem, mem + dim_x_ * dim_y_, mat_memory_);
    }

    void GetElemsToMem(T* mem) const {
        std::copy(mat_memory_, mat_memory_ + dim_x_ * dim_y_, mem);
    }

    template <typename Func>
    Matrix Apply(Func func);

    void NormalizeTo1();
};

template<typename T>
Matrix<T> operator*(const Matrix<T>& matrix_a, const Matrix<T>& matrix_b);

template<typename T>
Matrix<T> operator+(const Matrix<T>& matrix_a, const Matrix<T>& matrix_b);

template <typename T, typename U>
Matrix<T> Convolution(const Matrix<T>& target, const Matrix<U>& kernel);

// impl

template <typename T>
Matrix<T>::Matrix(const size_t dim_x, const size_t dim_y, const T* matrix_array) 
    : dim_x_{dim_x}, dim_y_{dim_y}, mat_memory_{new T[dim_x * dim_y]{}}
{
    spdlog::trace("Matrix constructor call: {:p} {}x{}", reinterpret_cast<const void*>(matrix_array), dim_x, dim_y);

    size_t size = dim_x * dim_y;
    if (matrix_array != nullptr) {
        std::copy(matrix_array, matrix_array + size, mat_memory_);
    }
}

template <typename T>
Matrix<T>::Matrix(const size_t dim_x, const size_t dim_y, const std::initializer_list<T>& matrix_list) 
    : dim_x_{dim_x}, dim_y_{dim_y}, mat_memory_{new T[dim_x * dim_y]{}}
{
    spdlog::trace("Matrix constructor call: {} {}x{}", matrix_list, dim_x, dim_y); // FIXME support initializer_list format

    std::copy(matrix_list.begin(), matrix_list.end(), mat_memory_);
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& mat) 
    : dim_x_{mat.dim_x_}, dim_y_{mat.dim_y_}, mat_memory_{new T[mat.dim_x_ * mat.dim_y_]{}}
{
    spdlog::trace("matrix copy constructor");

    std::copy(mat.mat_memory_, mat.mat_memory_ + mat.dim_x_ * mat.dim_y_, mat_memory_);
}

template <typename T> template <typename U>
Matrix<T>::Matrix(const Matrix<U>& mat)
    : dim_x_{mat.dim_x_}, dim_y_{mat.dim_y_}, mat_memory_{new T[mat.dim_x_ * mat.dim_y_]{}}
{
    spdlog::trace("matrix copy constructor");

    std::copy(mat.mat_memory_, mat.mat_memory_ + mat.dim_x_ * mat.dim_y_, mat_memory_);
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& mat) {
    spdlog::trace("matrix copy assignment");
    if (this == &mat) {
        return *this;
    }

    delete[] mat_memory_;
    size_t size = mat.dim_x_ * mat.dim_y_;
    mat_memory_ = new T[size];
    std::copy(mat.mat_memory_, mat.mat_memory_ + size, mat_memory_);

    return *this;
}

template <typename T> template <typename U>
Matrix<T>& Matrix<T>::operator=(const Matrix<U>& mat) {
    spdlog::trace("matrix copy assignment");
    if (this == &mat) {
        return *this;
    }

    delete[] mat_memory_;
    size_t size = mat.dim_x_ * mat.dim_y_;
    mat_memory_ = new T[size];
    std::copy(mat.mat_memory_, mat.mat_memory_ + size, mat_memory_);

    return *this;   
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(std::initializer_list<T> matrix_list) {
    spdlog::trace("matrix copy assignment");

    delete[] mat_memory_;
    mat_memory_ = new T[matrix_list.size()];
    std::copy(matrix_list.begin(), matrix_list.end(), mat_memory_);

    return *this;
}

template <typename T> template <typename Func>
Matrix<T> Matrix<T>::Apply(Func func) {
    spdlog::trace("Matrix<T>::Apply call");
    size_t dim_x = dim_x_;
    size_t dim_y = dim_y_;

    Matrix<T> new_mat{dim_x, dim_y};
    for (size_t i = 0; i < dim_x; i++) {
        for (size_t j = 0; j < dim_y; j++) {
            T value = T{func(this->GetElem(i, j))};
            new_mat.SetElem(i, j, value);
        }
    }

    return new_mat;
}

//    n            p         p
// m      *   n        = m 
// 
template <typename T>
Matrix<T> operator*(const Matrix<T>& matrix_a, const Matrix<T>& matrix_b) {
    if (matrix_a.GetDimX() != matrix_b.GetDimY()) { 
        spdlog::error("invalid matrix sizes for multiplication"); 
        throw MatrixException::InvalidMultiplication;
    }

    Matrix<T> matrix_c = Matrix<T>(matrix_b.GetDimX(), matrix_a.GetDimY());

    size_t dim_n = matrix_a.GetDimX();
    size_t dim_m = matrix_a.GetDimY();
    size_t dim_p = matrix_b.GetDimX();

    for (size_t j = 0; j < dim_m; j++) {
        for (size_t i = 0; i < dim_p; i++) {
            // inner loop
            for (size_t index_sum = 0; index_sum < dim_n; index_sum++) {
                T new_value = matrix_c.GetElem(i, j)
                              + matrix_a.GetElem(index_sum, j) 
                                * matrix_b.GetElem(i, index_sum);
                matrix_c.SetElem(i, j, new_value);
            }
        }
    }

    return matrix_c;
}

template<typename T>
Matrix<T> operator+(const Matrix<T>& matrix_a, const Matrix<T>& matrix_b) {
    spdlog::trace("Matrix<T>operator+ call");
    if ((matrix_a.GetDimX() != matrix_b.GetDimX()) || (matrix_a.GetDimY() != matrix_b.GetDimY())) {
        spdlog::error("invalid matrix sizes for multiplication"); 
        throw MatrixException::InvalidAddition;
    }

    size_t dim_x = matrix_a.GetDimX();
    size_t dim_y = matrix_b.GetDimY();

    Matrix<T> matrix_c = Matrix<T>(dim_x, dim_y);
    for (size_t i = 0; i < dim_x; i++) {
        for (size_t j = 0; j < dim_y; j++) {
            T sum = matrix_a.GetElem(i, j) + matrix_b.GetElem(i, j);
            matrix_c.SetElem(i, j, sum);
        }
    }

    return matrix_c;
}

template <typename T, typename U>
Matrix<T> Convolution(const Matrix<T>& target, const Matrix<U>& kernel) {
    spdlog::debug("Convolution ...");
    int64_t idim_x = target.GetDimX();
    int64_t idim_y = target.GetDimY();

    int64_t kdim_x = kernel.GetDimX();
    int64_t kdim_y = kernel.GetDimY();

    Matrix<T> res_mat(idim_x, idim_y);

    for (int64_t i = 0; i < idim_x; i++) {
        for (int64_t j = 0; j < idim_y; j++) {
            T sum{};
            for (int64_t k = 0; k < kdim_x; k++) {
                for (int64_t l = 0; l < kdim_y; l++) {
                    int64_t current_dim_x = i + k - (kdim_x / 2);
                    int64_t current_dim_y = j + l - (kdim_y / 2);

                    if (current_dim_y >= 0 && current_dim_y < idim_y && current_dim_x >= 0 && current_dim_x < idim_x) {
                        T add_value = target.GetElem(current_dim_x, current_dim_y) * kernel.GetElem(k, l);
                        // spdlog::trace("[i:{}, j:{}, k:{}, l:{}], add value: {}", i, j, k, l, add_value);
                        // spdlog::trace("kernel[k, l]: {}", kernel.GetElem(k, l));
                        // spdlog::trace("target[x, y]: {}", target.GetElem(current_dim_x, current_dim_y));
                        // spdlog::trace("current x, y: {}, {}", current_dim_x, current_dim_y);
                        sum += add_value;
                    }
                }
            }
            
            spdlog::trace("[{}, {}] sum value: {}", i, j, sum);
            res_mat.SetElem(i, j, sum);
        }
    }

    return res_mat;
}

template <typename T>
void Matrix<T>::NormalizeTo1() {
    spdlog::trace("Matrix norm to 1:");

    T sum{};

    for (size_t i = 0; i < dim_x_; i++) {
        for (size_t j = 0; j < dim_y_; j++) {
            sum += GetElem(i, j);
        }
    }

    for (size_t i = 0; i < dim_x_; i++) {
        for (size_t j = 0; j < dim_y_; j++) {
            T new_value = GetElem(i, j) / sum;
            SetElem(i, j, new_value);
        }
    }
}

} // namespace fcy

// format for matrix

template <typename T>
struct fmt::formatter<fcy::Matrix<T>> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const fcy::Matrix<T>& matrix, FormatContext& ctx) const -> decltype(ctx.out()) {
        auto out = ctx.out();
        size_t dim_x = matrix.GetDimX();
        size_t dim_y = matrix.GetDimY();

        fmt::format_to(out, "Matrix ({}x{}):\n", dim_x, dim_y);

        for (size_t j = 0; j < dim_y; ++j) {
            fmt::format_to(out, "[ ");
            for (size_t i = 0; i < dim_x; ++i) {
                fmt::format_to(out, "{} ", matrix.GetElem(i, j));
            }
            fmt::format_to(out, "]\n");
        }
        return out;
    }
};

#endif // MATRIX_HPP_
