#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <cstddef>
#include <cstring>

#include "spdlog/spdlog.h"

namespace fcy {

enum class MatrixException {
    Ok                    = 0,
    UninitEnum            = 1,
    InvalidMultiplication = 2,
    OutOfRange            = 3,
};

template <typename T>
class Matrix {
  private:
    size_t dim_x_;
    size_t dim_y_;
    T* mat_memory_;
  public:
    Matrix(const size_t dim_x, const size_t dim_y, const T* matrix_array = nullptr);
    
    Matrix(const Matrix& mat) 
        : dim_x_{mat.dim_x_}, 
          dim_y_{mat.dim_y_},
          mat_memory_{new T[mat.dim_x_ * mat.dim_y_]}
    {
        spdlog::trace("matrix copy constructor");
        std::copy(mat.mat_memory_, mat.mat_memory_ + mat.dim_x_ * mat.dim_y_, mat_memory_);
    }

    Matrix& operator=(const Matrix& mat) {
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

    ~Matrix() {
        delete[] mat_memory_;
    }
    

    size_t GetDimX() const { return dim_x_; };
    size_t GetDimY() const { return dim_y_; };

    // const T* GetData() const {return mat_memory_; }; // NOTE should i do this?
    T GetElem(size_t index_i, size_t index_j) const { 
        if ((index_i > dim_x_) || (index_j > dim_y_)) {
            spdlog::error("matrix get elem out of rang: i:{}, j:{}, dim x:{}, dim y:{}", index_i, index_j, dim_x_, dim_y_);
            throw MatrixException::OutOfRange;
        }

        return mat_memory_[index_j * dim_x_ + index_i];
    }

    void SetElem(size_t index_i, size_t index_j, T elem) {
        if ((index_i > dim_x_) || (index_j > dim_y_)) {
            spdlog::error("matrix set elem out of range: i:{}, j:{}, dim x:{}, dim y:{}", index_i, index_j, dim_x_, dim_y_);
            throw MatrixException::OutOfRange;
        }

        mat_memory_[index_j * dim_x_ + index_i] = elem;
    }

    void SetElemsFromMem(const T* mem) {
        std::copy(mem, mem + dim_x_ * dim_y_, mat_memory_);
    }

    void GetElemsToMem(T* mem) const {
        std::copy(mat_memory_, mat_memory_ + dim_x_ * dim_y_, mem);
    }
};

template<typename T>
Matrix<T> operator*(const Matrix<T>& matrix_a, const Matrix<T>& matrix_b);

template <typename T>
Matrix<T>::Matrix(const size_t dim_x, const size_t dim_y, const T* matrix_array) {
    spdlog::trace("Matrix constructor call: {:p} {}x{}", reinterpret_cast<const void*>(matrix_array), dim_x, dim_y);

    mat_memory_ = nullptr;
    dim_x_ = 0;
    dim_y_ = 0;   

    mat_memory_ = new T[dim_x * dim_y]{};
    dim_x_ = dim_x;
    dim_y_ = dim_y;

    size_t size = dim_x * dim_y;
    if (matrix_array != nullptr) {
        // memcpy(mat_memory_, matrix_array, dim_x * dim_y * sizeof(T));
        std::copy(matrix_array, matrix_array + size, mat_memory_);
    }
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

} // namespace fcy

#endif // MATRIX_HPP_
