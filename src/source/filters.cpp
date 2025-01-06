#include "filters.hpp"

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <climits>
#include <cstring>

#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "matrix.hpp"

// static -----------------------------------------------------------------------------------------

// Filters ----------------------------------------------------------------------------------------

void Filters::ReversFilter(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    assert(image != nullptr);
 
    for (Pixel* iter_pixels = image; iter_pixels < image + image_size_x * image_size_y; iter_pixels++) {
        // iter_pixels->red = UCHAR_MAX - iter_pixels->red;
        // iter_pixels->green = UCHAR_MAX - iter_pixels->green;
        // iter_pixels->blue = UCHAR_MAX - iter_pixels->blue;
        iter_pixels->SetRedColor(UCHAR_MAX - iter_pixels->GetRedColor());
        iter_pixels->SetGreenColor(UCHAR_MAX - iter_pixels->GetGreenColor());
        iter_pixels->SetBlueColor(UCHAR_MAX - iter_pixels->GetBlueColor());
    }
}

void Filters::ThresholdFilter(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    assert(image != nullptr);

}

void Filters::GaussianBlur(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    assert(image != nullptr);


    const size_t matrix_dim_x = 3;
    const size_t matrix_dim_y = 3;
    
    Pixel form_matrix[matrix_dim_x * matrix_dim_y] = {};

    Pixel convolution_matrix_mem[matrix_dim_x * matrix_dim_y] = {};
    Matrix<Pixel> convolution_matrix(matrix_dim_x, matrix_dim_y, convolution_matrix_mem);

    for (size_t j = 0; j < image_size_y - matrix_dim_y + 1; j++) {
        for (size_t i = 0; i < image_size_x - matrix_dim_x + 1; i++) {
            memcpy(form_matrix, image + j * image_size_x + i, sizeof(Pixel) * matrix_dim_x);      
            memcpy(form_matrix, image + (j + 1) * image_size_x + i, sizeof(Pixel) * matrix_dim_x);   
            memcpy(form_matrix, image + (j + 2) * image_size_x + i, sizeof(Pixel) * matrix_dim_x);     
            Matrix<Pixel> image_matrix(matrix_dim_x, matrix_dim_y, form_matrix);

            Matrix<Pixel> blur_matrix = convolution_matrix * image_matrix;
        }
    }

}

// static -----------------------------------------------------------------------------------------

