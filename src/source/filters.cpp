#include "filters.hpp"

#include <cstddef>
#include <climits>
#include <cstring>

#include "spdlog/spdlog.h"

#include "pixel.hpp"
#include "matrix.hpp"
#include "fcy_assert.hpp"

namespace fcy {

// static -----------------------------------------------------------------------------------------

static void ReversFilter_(Pixel* image, size_t image_size_x, size_t image_size_y);
static void ThresholdFilter_(Pixel* image, size_t image_size_x, size_t image_size_y);
static void GaussianBlur_(Pixel* image, size_t image_size_x, size_t image_size_y);

// Filters ----------------------------------------------------------------------------------------

void ReverseFilter::operator()(Pixel* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    ReversFilter_(pixel_buf, width, height);    
}

void GaussianBlurFilter::operator()(Pixel* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    GaussianBlur_(pixel_buf, width, height);    
}

void ThresholdFilter::operator()(Pixel* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    ThresholdFilter_(pixel_buf, width, height);    
}

// static -----------------------------------------------------------------------------------------

static void ReversFilter_(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    fcy_assert(image != nullptr);
 
    for (Pixel* iter_pixels = image; iter_pixels < image + image_size_x * image_size_y; iter_pixels++) {
        // iter_pixels->red = UCHAR_MAX - iter_pixels->red;
        // iter_pixels->green = UCHAR_MAX - iter_pixels->green;
        // iter_pixels->blue = UCHAR_MAX - iter_pixels->blue;
        iter_pixels->SetRedColor(UCHAR_MAX - iter_pixels->GetRedColor());
        iter_pixels->SetGreenColor(UCHAR_MAX - iter_pixels->GetGreenColor());
        iter_pixels->SetBlueColor(UCHAR_MAX - iter_pixels->GetBlueColor());
    }
}

static void ThresholdFilter_(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    fcy_assert(image != nullptr);
    
    (void)image_size_x;
    (void)image_size_y;
}

static void GaussianBlur_(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    fcy_assert(image != nullptr);

    const size_t matrix_dim_x = 3;
    const size_t matrix_dim_y = 3;
    
    Pixel form_matrix[matrix_dim_x * matrix_dim_y] = {};

    Pixel convolution_matrix_mem[matrix_dim_x * matrix_dim_y] = {};
    for (size_t i = 0; i < matrix_dim_x; i++) {
        for (size_t j = 0; j < matrix_dim_y; j++) {
            convolution_matrix_mem[j * matrix_dim_x + i] = Pixel(28, 28, 28, 255);
        }
    }

    Matrix<Pixel> convolution_matrix(matrix_dim_x, matrix_dim_y, convolution_matrix_mem);

    for (size_t j = 0; j < image_size_y - matrix_dim_y + 1; j++) {
        for (size_t i = 0; i < image_size_x - matrix_dim_x + 1; i++) {
            memcpy(&form_matrix[0 * matrix_dim_x], image + j * image_size_x + i, sizeof(Pixel) * matrix_dim_x);      
            memcpy(&form_matrix[1 * matrix_dim_x], image + (j + 1) * image_size_x + i, sizeof(Pixel) * matrix_dim_x);   
            memcpy(&form_matrix[2 * matrix_dim_x], image + (j + 2) * image_size_x + i, sizeof(Pixel) * matrix_dim_x);     
            Matrix<Pixel> image_matrix(matrix_dim_x, matrix_dim_y, form_matrix);

            Matrix<Pixel> blur_matrix = convolution_matrix * image_matrix;

            // FIXME
            // memcpy(form_matrix, image + (j + 0) * image_size_x + i, sizeof(Pixel) * matrix_dim_x);      
            // memcpy(form_matrix, image + (j + 1) * image_size_x + i, sizeof(Pixel) * matrix_dim_x);   
            // memcpy(form_matrix, image + (j + 2) * image_size_x + i, sizeof(Pixel) * matrix_dim_x);
            for (size_t copy_i = 0; copy_i < matrix_dim_x; copy_i++) {
                for (size_t copy_j = 0; copy_j < matrix_dim_y; copy_j++ ) {
                    form_matrix[copy_j * matrix_dim_x + copy_i] = blur_matrix.GetElem(copy_i, copy_j);
                }
            }

            memcpy(image + (j + 0) * image_size_x + i, &form_matrix[0 * matrix_dim_x], sizeof(Pixel) * matrix_dim_x);      
            memcpy(image + (j + 1) * image_size_x + i, &form_matrix[1 * matrix_dim_x], sizeof(Pixel) * matrix_dim_x);   
            memcpy(image + (j + 2) * image_size_x + i, &form_matrix[2 * matrix_dim_x], sizeof(Pixel) * matrix_dim_x);     
        }
    }
}

} // namespace fcy
