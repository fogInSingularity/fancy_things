#include "filters.hpp"

#include <cmath>
#include <cstddef>
#include <climits>
#include <cstring>

#include "spdlog/spdlog.h"

#include "pixel.hpp"
#include "matrix.hpp"
#include "fcy_assert.hpp"

namespace fcy {

// Filters ----------------------------------------------------------------------------------------

void ReverseFilter::operator()(PixelU* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    for (PixelU* iter_pixels = pixel_buf; iter_pixels < pixel_buf + width * height; iter_pixels++) {
        iter_pixels->SetRedColor(UCHAR_MAX - iter_pixels->GetRedColor());
        iter_pixels->SetGreenColor(UCHAR_MAX - iter_pixels->GetGreenColor());
        iter_pixels->SetBlueColor(UCHAR_MAX - iter_pixels->GetBlueColor());
    }
}

void BoxBlurFilter::operator()(PixelU* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    (void)width;
    (void)height;

    // const size_t conv_dim_x = 15;
    // const size_t conv_dim_y = 15;

    // Matrix<float> conv_mat(conv_dim_x, conv_dim_y);
    // for (size_t i = 0; i < conv_dim_x; i++) {
    //     for (size_t j = 0; j < conv_dim_y; j++) {
    //         conv_mat.SetElem(i, j, 1.0 / (conv_dim_x * conv_dim_y));
    //     }
    // }

    // Matrix<Pixel> image_mat(width, height, pixel_buf);

    // Matrix<Pixel> blured_image_mat = Convolution(image_mat, conv_mat);

    // blured_image_mat.GetElemsToMem(pixel_buf);
    // spdlog::error("box blur is still unfixed");
}

void ThresholdFilter::operator()(PixelU* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    (void)width;
    (void)height;

    spdlog::error("threshold filter not implemented yet");
}

void EdgeDetectorFilter::operator()(PixelU* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    const size_t conv_dim_x = 3;
    const size_t conv_dim_y = 3;
   
    Matrix<float> grad_x_ker(conv_dim_x, conv_dim_y);
    grad_x_ker = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1,
    };

    Matrix<float> grad_y_ker(conv_dim_x, conv_dim_y);
    grad_y_ker = {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1,
    };

    spdlog::trace("convert PixelU to GSPixel<float>:");
    GSPixel<float>* gspixel_buf = new GSPixel<float>[width * height];
    for (size_t i = 0; i < width * height; i++) {
        gspixel_buf[i] = PixelToGSPixel(Pixel<float>{pixel_buf[i]});
    }

    spdlog::trace("create matrix of gspixels:");
    Matrix<GSPixel<float>> image_mat(width, height, gspixel_buf);

    Matrix<GSPixel<float>> grad_x = Convolution(image_mat, grad_x_ker);
    Matrix<GSPixel<float>> grad_y = Convolution(image_mat, grad_y_ker);
        
    float max_gray = 0;
    float min_gray = 10000;
    
    auto grad_x2 = grad_x.Apply([](GSPixel<float> x){ return x.GetGray() * x.GetGray(); });
    auto grad_y2 = grad_y.Apply([](GSPixel<float> x){ return x.GetGray() * x.GetGray(); });

    Matrix<GSPixel<float>> grad_magn = (grad_x2 + grad_y2).Apply([](GSPixel<float> x){ return sqrtf(x.GetGray()); });

    spdlog::trace("Normalize:");

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            float value = grad_magn.GetElem(i, j).GetGray();
            max_gray = std::max(value, max_gray);
            min_gray = std::min(value, min_gray);
        }
    }

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            float value = grad_magn.GetElem(i, j).GetGray();
            if (value < 0) {
                fcy_assert(0 && "negative value after convolution");
            }
            float display_gray = ((value - min_gray) / (max_gray - min_gray)) * 255;
            spdlog::trace("[{}, {}] display gray: {}", i, j, display_gray);
            grad_magn.SetElem(i, j, GSPixel<float>{display_gray});
        }
    }

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            pixel_buf[j * width + i] = PixelU{GSPixelToPixel(grad_magn.GetElem(i, j))};
        }
    }

    delete[] gspixel_buf;
}

} // namespace fcy
