#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <cstddef>

template <class T>
class IMat3x3 {
  private:
    static const size_t kMatrixHeight_ = 3;
    static const size_t kMatrixWidth_  = 3;
    T mat_memory[kMatrixHeight_ * kMatrixWidth_];
  public:
    explicit IMat3x3(const T* matrix_array);
};

#endif // MATRIX_HPP_
