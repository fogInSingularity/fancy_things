#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <cstddef>
#include <cstring>

#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

enum class MatrixException {
    Ok                    = -1,
    UninitEnum            =  0,
    InvalidMultiplication =  1,
};

template <class T>
class Matrix {
  private:
    size_t dim_x_;
    size_t dim_y_;
    T* mat_memory_;
  public:
    Matrix(const size_t dim_x, const size_t dim_y, const T* matrix_array);
    Matrix(const size_t dim_x, const size_t dim_y);
    ~Matrix() = default;

    size_t GetDimX() const;
    size_t GetDimY() const;

    const T* GetData() const;

    void SetValues(const T* matrix_values) noexcept; 

    template<class U>
    friend Matrix<U> operator*(const Matrix<U>& matrix_a, const Matrix<U>& matrix_b);
};

// static ---------------------------------------------------------------------

template <class T>
static void MatrixMultiply(T* matrix_c, const T* matrix_a, const T* matrix_b, 
                           size_t dim_m, size_t dim_n, size_t dim_p);

// global ---------------------------------------------------------------------

template <class T>
Matrix<T>::Matrix(const size_t dim_x, const size_t dim_y, const T* matrix_array) {
    assert(matrix_array != nullptr);

    mat_memory_ = nullptr;
    dim_x_ = 0;
    dim_y_ = 0;   

    mat_memory_ = new T[dim_x * dim_y];
    dim_x_ = dim_x;
    dim_y_ = dim_y;
    memcpy(mat_memory_, matrix_array, dim_x * dim_y * sizeof(T));
}

template <class T>
Matrix<T>::Matrix(const size_t dim_x, const size_t dim_y) {
    mat_memory_ = nullptr;
    dim_x_ = 0;
    dim_y_ = 0;

    mat_memory_ = new T[dim_x * dim_y];
    dim_x_ = dim_x;
    dim_y_ = dim_y;
    memset(mat_memory_, 0, dim_x * dim_y * sizeof(T));
}

template<class T>
size_t Matrix<T>::GetDimX() const {
    return dim_x_;
}

template<class T>
size_t Matrix<T>::GetDimY() const {
    return dim_y_;
}

template<class T>
const T* Matrix<T>::GetData() const {
    return mat_memory_;
}

template<class T>
void Matrix<T>::SetValues(const T* matrix_values) noexcept {
    memcpy(mat_memory_, matrix_values, dim_x_ * dim_y_ * sizeof(T));
}

template <class T>
Matrix<T> operator*(const Matrix<T>& matrix_a, const Matrix<T>& matrix_b) {
    if (matrix_a.dim_x_ != matrix_b.dim_y_) { 
        spdlog::error("invalid matrix sizes for multiplication"); 
        throw MatrixException::InvalidMultiplication;
    }

    Matrix<T> matrix_c = Matrix<T>(matrix_b.dim_x_, matrix_a.dim_y_);
    MatrixMultiply<T>(matrix_c.mat_memory_, 
                      matrix_a.mat_memory_, 
                      matrix_b.mat_memory_, 
                      matrix_a.dim_y_, 
                      matrix_a.dim_x_, 
                      matrix_b.dim_x_);

    return matrix_c;
}

// static ---------------------------------------------------------------------

//    n            p         p
// m      *   n        = m 
// 

template <class T>
static void MatrixMultiply(T* matrix_c, const T* matrix_a, const T* matrix_b, 
                           size_t dim_m, size_t dim_n, size_t dim_p) 
{
    assert(matrix_c != nullptr);
    assert(matrix_a != nullptr);
    assert(matrix_b != nullptr);

    for (size_t i = 0; i < dim_m; i++) {
        for (size_t j = 0; j < dim_p; j++) {
            // inner loop
            // matrix_c[i * dim_p + j] = 0;  
            std::memset(&matrix_c[i * dim_p + j], 0, sizeof(T));
            for (size_t index_sum = 0; index_sum < dim_n; index_sum++) {
                matrix_c[i * dim_p + j] += matrix_a[i * dim_n + index_sum] 
                                           * matrix_b[index_sum * dim_p + j];
            } 
        }
    }
}

#endif // MATRIX_HPP_
