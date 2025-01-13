#include "filters.hpp"

#include <cmath>
#include <cstddef>
#include <climits>
#include <cstdint>
#include <numbers>
#include <type_traits>

#include "spdlog/spdlog.h"

#include "pixel.hpp"
#include "matrix.hpp"
#include "fcy_assert.hpp"

namespace fcy {

static size_t ConvertIJtoLinIndex(size_t i, size_t j, size_t width, size_t height);

template <typename T>
static Matrix<T> Normalize(const Matrix<T>& mat);

template <typename T, typename U>
static Matrix<GSPixel<U>> ConvertRGBImageToGrayScaleMat(const Pixel<T>* pixel_buf, size_t width, size_t height);

template <typename T, typename U>
static void ConvertRGBImageToGrayScaleChanelsMats(const Pixel<T>* pixel_buf, size_t width, size_t height, 
                                                  Matrix<GSPixel<U>>* red_ch_mat, Matrix<GSPixel<U>>* green_ch_mat, Matrix<GSPixel<U>>* blue_ch_mat);

template <typename T, typename U>
static void ConvertGrayScaleMatToRGBImage(const Matrix<GSPixel<U>>& gspixel_mat, Pixel<T>* pixel_buf);

template <typename T, typename U>
static void ConvertGrayScaleChanelsMatsToRGSImage(Matrix<GSPixel<U>>* red_ch_mat, Matrix<GSPixel<U>>* green_ch_mat, Matrix<GSPixel<U>>* blue_ch_mat,
                                                  Pixel<T>* pixel_buf, size_t width, size_t height);

template <typename T>
static T NormalDistributionCurve(T mean_x, T mean_y, T stddev, T x, T y);

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

    // https://en.wikipedia.org/wiki/Box_blur

    const size_t ker_dim_x = 5;
    const size_t ker_dim_y = 5;

    Matrix<float> blur_ker(ker_dim_x, ker_dim_y);
    for (size_t i = 0; i < ker_dim_x; i++) {
        for (size_t j = 0; j < ker_dim_y; j++) {
            blur_ker.SetElem(i, j, 1.0);
        }
    }

    blur_ker.NormalizeTo1();

    Matrix<GSPixel<float>> red_ch_mat(width, height);
    Matrix<GSPixel<float>> green_ch_mat(width, height);
    Matrix<GSPixel<float>> blue_ch_mat(width, height);

    ConvertRGBImageToGrayScaleChanelsMats(pixel_buf, width, height, &red_ch_mat, &green_ch_mat, &blue_ch_mat);

    auto conv_red_mat   = Convolution(red_ch_mat, blur_ker);
    auto conv_green_mat = Convolution(green_ch_mat, blur_ker);
    auto conv_blue_mat  = Convolution(blue_ch_mat, blur_ker);

    ConvertGrayScaleChanelsMatsToRGSImage(&conv_red_mat, &conv_green_mat, &conv_blue_mat, pixel_buf, width, height);
}

void GaussianBlurFilter::operator()(PixelU* pixel_buf, size_t width, size_t height) { 
    fcy_assert(pixel_buf != nullptr);

    // https://en.wikipedia.org/wiki/Gaussian_blur

    const size_t ker_dim_x = 3;
    const size_t ker_dim_y = 3;

    float mean = 0;
    float stddev = 0.5;

    Matrix<float> blur_ker(ker_dim_x, ker_dim_y);
    for (size_t i = 0; i < ker_dim_x; i++) {
        for (size_t j = 0; j < ker_dim_y; j++) {
            int64_t x = static_cast<int64_t>(i) - ker_dim_x / 2;
            int64_t y = static_cast<int64_t>(i) - ker_dim_y / 2;

            float normal_value = NormalDistributionCurve<float>(mean, mean, stddev, x, y);
            spdlog::trace("[i:{}, j:{}] normal value: {}", i, j, normal_value);
            blur_ker.SetElem(i, j, normal_value);
        }
    }

    blur_ker.NormalizeTo1();

    Matrix<GSPixel<float>> red_ch_mat(width, height);
    Matrix<GSPixel<float>> green_ch_mat(width, height);
    Matrix<GSPixel<float>> blue_ch_mat(width, height);

    ConvertRGBImageToGrayScaleChanelsMats(pixel_buf, width, height, &red_ch_mat, &green_ch_mat, &blue_ch_mat);

    auto conv_red_mat   = Convolution(red_ch_mat, blur_ker);
    auto conv_green_mat = Convolution(green_ch_mat, blur_ker);
    auto conv_blue_mat  = Convolution(blue_ch_mat, blur_ker);

    ConvertGrayScaleChanelsMatsToRGSImage(&conv_red_mat, &conv_green_mat, &conv_blue_mat, pixel_buf, width, height);
}


void MotionBlurFilter::operator()(PixelU* pixel_buf, size_t width, size_t height) { 
    fcy_assert(pixel_buf != nullptr);

    const size_t conv_dim_x = 15;
    const size_t conv_dim_y = 15;

    Matrix<float> motion_ker(conv_dim_x, conv_dim_y);
    for (size_t i = 0; i < conv_dim_x; i++) {
        motion_ker.SetElem(i, i, 1);
    }
    motion_ker.NormalizeTo1();

    Matrix<GSPixel<float>> red_ch_mat(width, height);
    Matrix<GSPixel<float>> green_ch_mat(width, height);
    Matrix<GSPixel<float>> blue_ch_mat(width, height);

    ConvertRGBImageToGrayScaleChanelsMats(pixel_buf, width, height, &red_ch_mat, &green_ch_mat, &blue_ch_mat);

    auto conv_red_mat   = Convolution(red_ch_mat, motion_ker);
    auto conv_green_mat = Convolution(green_ch_mat, motion_ker);
    auto conv_blue_mat  = Convolution(blue_ch_mat, motion_ker);

    ConvertGrayScaleChanelsMatsToRGSImage(&conv_red_mat, &conv_green_mat, &conv_blue_mat, pixel_buf, width, height);
}

void ThresholdFilter::operator()(PixelU* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    (void)width;
    (void)height;

    spdlog::error("threshold filter not implemented yet");
}

void EmbossingFilter::operator()(PixelU* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    const size_t conv_dim_x = 3;
    const size_t conv_dim_y = 3;

    Matrix<float> emb_ker(conv_dim_x, conv_dim_y);
    emb_ker = {
        -2,  -1, 0,
        -1,   1, 1,
         0,   1, 2
    };

    Matrix<GSPixel<float>> gspixel_mat = ConvertRGBImageToGrayScaleMat<uint8_t, float>(pixel_buf, width, height);

    Matrix<GSPixel<float>> emb_mat = Convolution(gspixel_mat, emb_ker);

    Matrix<GSPixel<float>> norm_mat = Normalize(emb_mat);

    ConvertGrayScaleMatToRGBImage(norm_mat, pixel_buf);
}

void EdgeDetectorSobelFilter::operator()(PixelU* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    // Sobel operator
    // https://en.wikipedia.org/wiki/Sobel_operator
    
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

    Matrix<GSPixel<float>> gspixel_mat = ConvertRGBImageToGrayScaleMat<uint8_t, float>(pixel_buf, width, height);

    Matrix<GSPixel<float>> grad_x = Convolution(gspixel_mat, grad_x_ker);
    Matrix<GSPixel<float>> grad_y = Convolution(gspixel_mat, grad_y_ker);
        
    auto grad_x2 = grad_x.Apply([](GSPixel<float> x){ return x * x; });
    auto grad_y2 = grad_y.Apply([](GSPixel<float> x){ return x * x; });

    Matrix<GSPixel<float>> grad_magn = (grad_x2 + grad_y2).Apply([](GSPixel<float> x){ return sqrtf(x); });

    Matrix<GSPixel<float>> norm_mat = Normalize(grad_magn);

    ConvertGrayScaleMatToRGBImage(norm_mat, pixel_buf);
}

void EdgeDetectorLaplacianFilter::operator()(PixelU* pixel_buf, size_t width, size_t height) {
    fcy_assert(pixel_buf != nullptr);

    const size_t conv_dim_x = 3;
    const size_t conv_dim_y = 3;

    Matrix<float> laplace_ker(conv_dim_x, conv_dim_y);
    laplace_ker = {
        0,  1, 0,
        1, -4, 1,
        0,  1, 0
    };

    Matrix<GSPixel<float>> gspixel_mat = ConvertRGBImageToGrayScaleMat<uint8_t, float>(pixel_buf, width, height);

    Matrix<GSPixel<float>> laplace_mat = Convolution(gspixel_mat, laplace_ker);

    Matrix<GSPixel<float>> norm_mat = Normalize(laplace_mat);

    ConvertGrayScaleMatToRGBImage(norm_mat, pixel_buf);
}

// static -----------------------------------------------------------------------------------------

static size_t ConvertIJtoLinIndex(size_t i, size_t j, size_t width, size_t height) {
    (void)height;
    return j * width + i;
}

template <typename T> // double float
static Matrix<T> Normalize(const Matrix<T>& mat) {
    static_assert(std::is_arithmetic<T>());
    spdlog::trace("Normalize:");

    size_t width  = mat.GetDimX();
    size_t height = mat.GetDimY();

    T max_value = 0;
    T min_value = INFINITY;

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            T value = mat.GetElem(i, j);
            max_value = std::max(value, max_value);
            min_value = std::min(value, min_value);
        }
    }

    spdlog::trace("min value: {}, max value: {}", min_value, max_value);

    Matrix<T> norm_mat(width, height);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            T value = mat.GetElem(i, j);
            spdlog::trace("value: {}", value);
            // if (value < 0) {
            //     fcy_assert(0 && "negative value after convolution");
            // }

            T norm_value = ((value - min_value) / (max_value - min_value));
            spdlog::trace("[{}, {}] norm value: {}", i, j, norm_value);
            norm_mat.SetElem(i, j, T{norm_value});
        }
    }

    return norm_mat;
}

template <typename T, typename U>
static Matrix<GSPixel<U>> ConvertRGBImageToGrayScaleMat(const Pixel<T>* pixel_buf, size_t width, size_t height) {
    static_assert(std::is_arithmetic<T>());
    static_assert(std::is_arithmetic<U>());
    fcy_assert(pixel_buf != nullptr);

    spdlog::trace("ConvertRGBImageToGrayScale:");

    Matrix<GSPixel<U>> gspixel_mat(width, height);
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {    // NOTE -V-
            GSPixel<float> gspixel = PixelToGSPixel(Pixel<U>{pixel_buf[ConvertIJtoLinIndex(i, j, width, height)]});
            gspixel_mat.SetElem(i, j, gspixel);
        }     
    }

    return gspixel_mat;
}

template <typename T, typename U>
static void ConvertGrayScaleMatToRGBImage(const Matrix<GSPixel<U>>& gspixel_mat, Pixel<T>* pixel_buf) {
    static_assert(std::is_arithmetic<T>());
    static_assert(std::is_arithmetic<U>());
    fcy_assert(pixel_buf != nullptr);   

    spdlog::trace("ConvertGrayScaleMatToRGBImage");

    size_t width = gspixel_mat.GetDimX();
    size_t height = gspixel_mat.GetDimY();

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            pixel_buf[ConvertIJtoLinIndex(i, j, width, height)] = PixelU{GSPixelToPixel(gspixel_mat.GetElem(i, j))};
        }
    }
}

template <typename T, typename U>
static void ConvertRGBImageToGrayScaleChanelsMats(const Pixel<T>* pixel_buf, size_t width, size_t height, 
                                                  Matrix<GSPixel<U>>* red_ch_mat, Matrix<GSPixel<U>>* green_ch_mat, Matrix<GSPixel<U>>* blue_ch_mat) 
{
    static_assert(std::is_arithmetic<T>());
    static_assert(std::is_arithmetic<U>());
    fcy_assert(pixel_buf != nullptr);   
    fcy_assert(red_ch_mat != nullptr);
    fcy_assert(green_ch_mat != nullptr);
    fcy_assert(blue_ch_mat != nullptr); 

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            Pixel<T> pixel{pixel_buf[ConvertIJtoLinIndex(i, j, width, height)]};
            red_ch_mat->SetElem(i, j, GSPixel<U>{Pixel<U>{pixel}.GetRedColor()});
            green_ch_mat->SetElem(i, j, GSPixel<U>{Pixel<U>{pixel}.GetGreenColor()});
            blue_ch_mat->SetElem(i, j, GSPixel<U>{Pixel<U>{pixel}.GetBlueColor()});
        }
    }
}

template <typename T, typename U>
static void ConvertGrayScaleChanelsMatsToRGSImage(Matrix<GSPixel<U>>* red_ch_mat, Matrix<GSPixel<U>>* green_ch_mat, Matrix<GSPixel<U>>* blue_ch_mat,
                                                  Pixel<T>* pixel_buf, size_t width, size_t height) 
{
    static_assert(std::is_arithmetic<T>());
    static_assert(std::is_arithmetic<U>());
    fcy_assert(pixel_buf != nullptr);   
    fcy_assert(red_ch_mat != nullptr);
    fcy_assert(green_ch_mat != nullptr);
    fcy_assert(blue_ch_mat != nullptr); 

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            Pixel<T> pixel;

            pixel.SetRedColor(red_ch_mat->GetElem(i, j));
            pixel.SetGreenColor(green_ch_mat->GetElem(i, j));
            pixel.SetBlueColor(blue_ch_mat->GetElem(i, j));

            pixel_buf[ConvertIJtoLinIndex(i, j, width, height)] = pixel;
        }
    }   
}

template <typename T>
static T NormalDistributionCurve(T mean_x, T mean_y, T stddev, T x, T y) {
    // normal distribution in case of independent variables and curve itself is symetrical
    // https://en.wikipedia.org/wiki/Multivariate_normal_distribution
    // formula: f(x, y) = (1 / (2 * pi * stddev^2)) * exp(-0.5 * ((x - mean_x)^2 + (y - mean_y)^2) / stddev^2)
    auto sqr = [](T x){ return x * x; };
    T value_pre_exp = -0.5 * (sqr(x - mean_x) + sqr(y - mean_y)) / sqr(stddev);
    return (1 / (2 * std::numbers::pi_v<T> * sqr(stddev))) * std::exp(value_pre_exp);
}

} // namespace fcy
