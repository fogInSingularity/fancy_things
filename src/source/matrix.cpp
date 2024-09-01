#include "matrix.hpp"

#include <cstddef>
#include <cstring>
#include <cassert>

#include "logging.h"

// static ---------------------------------------------------------------------

template <class T>
static void MatrixMultiply(T* matrix_c, const T* matrix_a, const T* matrix_b, 
                           size_t dim_m, size_t dim_n, size_t dim_p);

// global ---------------------------------------------------------------------

template <class T>
IMat3x3<T>::IMat3x3(const T* matrix_array) noexcept {
    assert(matrix_array != nullptr);

    LogFunctionEntry();

    memcpy(mat_memory, matrix_array);
}

template <class T>
IMat3x3<T> operator*(const IMat3x3<T>& matrix_a, const IMat3x3<T>& matrix_b) {
    LogFunctionEntry();

    IMat3x3<T> matrix_c = {};

    static_assert(IMat3x3<T>::kMatrixHeight_ == 3, "matrix should be 3x3"); 
    static_assert(IMat3x3<T>::kMatrixWidth_  == 3, "matrix should be 3x3");

    size_t dim_m = IMat3x3<T>::kMatrixHeight_;
    size_t dim_n = IMat3x3<T>::kMatrixWidth_;
    size_t dim_p = IMat3x3<T>::kMatrixWidth_;

    MatrixMultiply<T>(matrix_c.mat_memory, matrix_a.mat_memory, matrix_b.mat_memory, dim_m, dim_n, dim_p);

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

    LogFunctionEntry();
    LogVariable("%p", matrix_c);
    LogVariable("%p", matrix_a);
    LogVariable("%p", matrix_b);

    LogVariable("%lu", dim_m);
    LogVariable("%lu", dim_n);
    LogVariable("%lu", dim_p);

    for (size_t i = 0; i < dim_m; i++) {
        for (size_t j = 0; j < dim_p; j++) {
            // inner loop
            for (size_t index_sum = 0; index_sum < dim_n; index_sum++) {
                matrix_c[i * dim_p + j] = matrix_a[i * dim_n + index_sum] * matrix_b[index_sum * dim_p + j];
            } 
        }
    }
}
