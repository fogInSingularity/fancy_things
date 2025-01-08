#include "filters.hpp"

#include <cstddef>
#include <climits>
#include <cstdint>
#include <cstring>

#include "spdlog/spdlog.h"

#include "pixel.hpp"
#include "matrix.hpp"
#include "fcy_assert.hpp"

namespace fcy {

// static -----------------------------------------------------------------------------------------

static void ReversFilter_(Pixel* image, size_t image_size_x, size_t image_size_y);
static void ThresholdFilter_(Pixel* image, size_t image_size_x, size_t image_size_y);
static void BoxBlur_(Pixel* image, size_t image_size_x, size_t image_size_y);

template <typename T, typename U>
static Matrix<T> Convolution(Matrix<T>* image, Matrix<U>* kernel);

// Filters ----------------------------------------------------------------------------------------

void ReverseFilter::operator()(Pixel* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    ReversFilter_(pixel_buf, width, height);    
}

void BoxBlurFilter::operator()(Pixel* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    BoxBlur_(pixel_buf, width, height);    
}

void ThresholdFilter::operator()(Pixel* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    ThresholdFilter_(pixel_buf, width, height);    
}

// static -----------------------------------------------------------------------------------------

static void ReversFilter_(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    fcy_assert(image != nullptr);
 
    for (Pixel* iter_pixels = image; iter_pixels < image + image_size_x * image_size_y; iter_pixels++) {
        iter_pixels->SetRedColor(UCHAR_MAX - iter_pixels->GetRedColor());
        iter_pixels->SetGreenColor(UCHAR_MAX - iter_pixels->GetGreenColor());
        iter_pixels->SetBlueColor(UCHAR_MAX - iter_pixels->GetBlueColor());
    }
}

static void ThresholdFilter_(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    fcy_assert(image != nullptr);
    
    (void)image_size_x;
    (void)image_size_y;

    spdlog::error("threshold filter not implemented yet");
}

static void BoxBlur_(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    fcy_assert(image != nullptr);

    const size_t conv_dim_x = 5;
    const size_t conv_dim_y = 5;
   
    Matrix<double> conv_mat(conv_dim_x, conv_dim_y);
    for (size_t i = 0; i < conv_dim_x; i++) {
        for (size_t j = 0; j < conv_dim_y; j++) {
            conv_mat.SetElem(i, j, 1.0 / (conv_dim_x * conv_dim_y));
        }
    }

    Matrix<Pixel> image_mat(image_size_x, image_size_y, image);

    Matrix<Pixel> blured_image_mat = Convolution(&image_mat, &conv_mat);

    blured_image_mat.GetElemsToMem(image);
}

template <typename T, typename U>
static Matrix<T> Convolution(Matrix<T>* image, Matrix<U>* kernel) {
    fcy_assert(image != nullptr);
    fcy_assert(kernel != nullptr);

    int64_t idim_x = image->GetDimX();
    int64_t idim_y = image->GetDimY();

    int64_t kdim_x = kernel->GetDimX();
    int64_t kdim_y = kernel->GetDimY();

    Matrix<T> res_mat(idim_x, idim_y);

    for (int64_t i = 0; i < idim_x; ++i) {
        for (int64_t j = 0; j < idim_y; ++j) {
            T sum;
            for (int64_t k = 0; k < kdim_x; ++k) {
                for (int64_t l = 0; l < kdim_y; ++l) {
                    int64_t current_dim_x = i + k - (kdim_x / 2);
                    int64_t current_dim_y = j + l - (kdim_y / 2);

                    if (current_dim_y >= 0 && current_dim_y < idim_y && current_dim_x >= 0 && current_dim_x < idim_x) {
                        sum += image->GetElem(current_dim_x, current_dim_y) * kernel->GetElem(k, l);
                    }
                }
            }

            res_mat.SetElem(i, j, sum);
        }
    }

    return res_mat;
}

} // namespace fcy
