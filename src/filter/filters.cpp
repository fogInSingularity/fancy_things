#include "filter/filters.hpp"

#include <cmath>
#include <cstddef>
#include <climits>
#include <cstdint>
#include <cassert>
#include <numbers>
#include <type_traits>

#include <spdlog/spdlog.h>

#include "filter/pixel.hpp"
#include "filter/matrix.hpp"
#include "filter/utility.hpp"
#include "filter/mat_op.hpp"

namespace ftr {

template <typename T>
static Matrix<T> Normalize(const Matrix<T>& mat);

template <typename T, typename U>
static Matrix<GSPixel<U>> ConvertRGBImageToGrayScaleMat(const Matrix<Pixel<T>>& image);

template <typename T, typename U>
static void ConvertRGBImageToGrayScaleChanelsMats(const Matrix<Pixel<T>>& rgb_image, 
                                                  Matrix<GSPixel<U>>* red_ch_mat, Matrix<GSPixel<U>>* green_ch_mat, Matrix<GSPixel<U>>* blue_ch_mat);

template <typename T, typename U>
static void ConvertGrayScaleMatToRGBImage(const Matrix<GSPixel<U>>& gs_image, Matrix<Pixel<T>>* rgb_image);

template <typename T, typename U>
static void ConvertGrayScaleChanelsMatsToRGSImage(Matrix<GSPixel<U>>* red_ch_mat, Matrix<GSPixel<U>>* green_ch_mat, Matrix<GSPixel<U>>* blue_ch_mat,
                                                  Matrix<Pixel<T>>* rgb_image);

template <typename T>
static T NormalDistributionCurve(T mean_x, T mean_y, T stddev, T x, T y);

// Filters ----------------------------------------------------------------------------------------

void ReverseFilter::operator()(Matrix<PixelU>* image) {
    assert(image != nullptr);

    Size size = image->GetSize();

    for (size_t i = 0; i < size.w; i++) {
        for (size_t j = 0; j < size.h; j++) {
            image->At(i, j) = PixelU{UCHAR_MAX, UCHAR_MAX, UCHAR_MAX} - image->At(i, j);
        }
    }
}

void BoxBlurFilter::operator()(Matrix<PixelU>* image) {
    assert(image != nullptr);

    // https://en.wikipedia.org/wiki/Box_blur

    Matrix<float> blur_ker{ker_size_};
    for (size_t i = 0; i < ker_size_.w; i++) {
        for (size_t j = 0; j < ker_size_.h; j++) {
            blur_ker.At(i, j) = 1.0;
        }
    }
    blur_ker = NormalizeTo1(blur_ker);

    Size mat_size = image->GetSize();

    Matrix<GSPixel<float>> red_ch_mat{mat_size};
    Matrix<GSPixel<float>> green_ch_mat{mat_size};
    Matrix<GSPixel<float>> blue_ch_mat{mat_size};

    ConvertRGBImageToGrayScaleChanelsMats(*image, &red_ch_mat, &green_ch_mat, &blue_ch_mat);

    auto conv_red_mat   = Convolution(red_ch_mat, blur_ker);
    auto conv_green_mat = Convolution(green_ch_mat, blur_ker);
    auto conv_blue_mat  = Convolution(blue_ch_mat, blur_ker);

    ConvertGrayScaleChanelsMatsToRGSImage(&conv_red_mat, &conv_green_mat, &conv_blue_mat, image);
}

void GaussianBlurFilter::operator()(Matrix<PixelU>* image) { 
    assert(image != nullptr);

    // https://en.wikipedia.org/wiki/Gaussian_blur

    Matrix<float> blur_ker{ker_size_};

    auto small_size = SizeT<int>{ker_size_};

    for (int i = 0; i < small_size.w; i++) {
        for (int j = 0; j < small_size.h; j++) {
            float x = static_cast<float>(i - small_size.w / 2);
            float y = static_cast<float>(j - small_size.h / 2);
            
            float normal_value = NormalDistributionCurve<float>(mean_, mean_, stddev_, x, y);
            spdlog::trace("[i:{}, j:{}] normal value: {}", i, j, normal_value);
            
            blur_ker.At(i, j) = normal_value;
        }
    }

    blur_ker = NormalizeTo1(blur_ker);

    Matrix<GSPixel<float>> red_ch_mat{image->GetSize()};
    Matrix<GSPixel<float>> green_ch_mat{image->GetSize()};
    Matrix<GSPixel<float>> blue_ch_mat{image->GetSize()};

    ConvertRGBImageToGrayScaleChanelsMats(*image, &red_ch_mat, &green_ch_mat, &blue_ch_mat);

    auto conv_red_mat   = Convolution(red_ch_mat, blur_ker);
    auto conv_green_mat = Convolution(green_ch_mat, blur_ker);
    auto conv_blue_mat  = Convolution(blue_ch_mat, blur_ker);

    ConvertGrayScaleChanelsMatsToRGSImage(&conv_red_mat, &conv_green_mat, &conv_blue_mat, image);
}

void MotionBlurFilter::operator()(Matrix<PixelU>* image) { 
    assert(image != nullptr);

    Matrix<float> motion_ker{ker_size_};
    for (size_t i = 0; i < ker_size_.w; i++) {
        motion_ker.At(i, i) = 1; // i = j
    }
    motion_ker = NormalizeTo1(motion_ker);

    Matrix<GSPixel<float>> red_ch_mat{image->GetSize()};
    Matrix<GSPixel<float>> green_ch_mat{image->GetSize()};
    Matrix<GSPixel<float>> blue_ch_mat{image->GetSize()};

    ConvertRGBImageToGrayScaleChanelsMats(*image, &red_ch_mat, &green_ch_mat, &blue_ch_mat);

    auto conv_red_mat   = Convolution(red_ch_mat, motion_ker);
    auto conv_green_mat = Convolution(green_ch_mat, motion_ker);
    auto conv_blue_mat  = Convolution(blue_ch_mat, motion_ker);

    ConvertGrayScaleChanelsMatsToRGSImage(&conv_red_mat, &conv_green_mat, &conv_blue_mat, image);
}

void ThresholdFilter::operator()(Matrix<PixelU>* image) {
    assert(image != nullptr);

    Matrix<float> grad_x_ker{ker_size_};
    grad_x_ker = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1,
    };

    Matrix<float> grad_y_ker{ker_size_};
    grad_y_ker = {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1,
    };

    Matrix<GSPixel<float>> gspixel_mat = ConvertRGBImageToGrayScaleMat<uint8_t, float>(*image);

    Matrix<GSPixel<float>> grad_x = Convolution(gspixel_mat, grad_x_ker);
    Matrix<GSPixel<float>> grad_y = Convolution(gspixel_mat, grad_y_ker);
    
    auto sqr_mat_elem = [](GSPixel<float> x){ return x * x; };
    auto grad_x2 = Apply(grad_x, sqr_mat_elem);
    auto grad_y2 = Apply(grad_y, sqr_mat_elem);

    auto sqrt_mat_elem = [](GSPixel<float> x){ return sqrtf(x); };
    Matrix<GSPixel<float>> grad_magn = Apply(grad_x2 + grad_y2, sqrt_mat_elem);

    Matrix<GSPixel<float>> norm_mat = Normalize(grad_magn);

    Size image_size = image->GetSize();
    Matrix<GSPixel<float>> final_mat{image_size};

    for (size_t i = 0; i < image_size.w; i++) {
        for (size_t j = 0; j < image_size.h; j++) {
            final_mat.At(i, j) = norm_mat.At(i, j) > threshold_ ? 1 : 0;
        }
    }

    ConvertGrayScaleMatToRGBImage(final_mat, image);

    spdlog::error("threshold filter not implemented yet");
}

void EmbossingFilter::operator()(Matrix<PixelU>* image) {
    assert(image != nullptr);

    Size ker_size = {3, 3};
    Matrix<float> emb_ker{ker_size};
    emb_ker = {
        -2,  -1, 0,
        -1,   1, 1,
         0,   1, 2
    };

    Matrix<GSPixel<float>> gspixel_mat = ConvertRGBImageToGrayScaleMat<uint8_t, float>(*image);

    Matrix<GSPixel<float>> emb_mat = Convolution(gspixel_mat, emb_ker);

    Matrix<GSPixel<float>> norm_mat = Normalize(emb_mat);

    ConvertGrayScaleMatToRGBImage(norm_mat, image);
}

void EdgeDetectorSobelFilter::operator()(Matrix<PixelU>* image) {
    assert(image != nullptr);

    // Sobel operator
    // https://en.wikipedia.org/wiki/Sobel_operator
    
    Size ker_size = {3, 3};

    Matrix<float> grad_x_ker{ker_size};
    grad_x_ker = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1,
    };

    Matrix<float> grad_y_ker{ker_size};
    grad_y_ker = {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1,
    };

    Matrix<GSPixel<float>> gspixel_mat = ConvertRGBImageToGrayScaleMat<uint8_t, float>(*image);

    Matrix<GSPixel<float>> grad_x = Convolution(gspixel_mat, grad_x_ker);
    Matrix<GSPixel<float>> grad_y = Convolution(gspixel_mat, grad_y_ker);
       
    auto sqr_mat_elem = [](GSPixel<float> x){ return x * x; };
    auto grad_x2 = Apply(grad_x, sqr_mat_elem);
    auto grad_y2 = Apply(grad_y, sqr_mat_elem);

    auto sqrt_mat_elem = [](GSPixel<float> x){ return sqrtf(x); };
    Matrix<GSPixel<float>> grad_magn = Apply(grad_x2 + grad_y2, sqrt_mat_elem);

    Matrix<GSPixel<float>> norm_mat = Normalize(grad_magn);

    ConvertGrayScaleMatToRGBImage(norm_mat, image);
}

void EdgeDetectorLaplacianFilter::operator()(Matrix<PixelU>* image) {
    assert(image != nullptr);

    Size ker_size = {3, 3};

    Matrix<float> laplace_ker{ker_size};
    laplace_ker = {
        0,  1, 0,
        1, -4, 1,
        0,  1, 0
    };

    Matrix<GSPixel<float>> gspixel_mat = ConvertRGBImageToGrayScaleMat<uint8_t, float>(*image);

    Matrix<GSPixel<float>> laplace_mat = Convolution(gspixel_mat, laplace_ker);

    Matrix<GSPixel<float>> norm_mat = Normalize(laplace_mat);

    ConvertGrayScaleMatToRGBImage(norm_mat, image);
}

// static -----------------------------------------------------------------------------------------

template <typename T> // double float
static Matrix<T> Normalize(const Matrix<T>& mat) {
    static_assert(std::is_arithmetic<T>());
    spdlog::trace("Normalize:");

    Size size = mat.GetSize();
    
    T max_value = 0;
    T min_value = INFINITY;

    for (size_t i = 0; i < size.w; i++) {
        for (size_t j = 0; j < size.h; j++) {
            T value = mat.At(i, j);
            max_value = std::max(value, max_value);
            min_value = std::min(value, min_value);
        }
    }

    spdlog::trace("min value: {}, max value: {}", min_value, max_value);

    Matrix<T> norm_mat{size};

    for (size_t i = 0; i < size.w; i++) {
        for (size_t j = 0; j < size.h; j++) {
            T value = mat.At(i, j);
            spdlog::trace("value: {}", value);
            // if (value < 0) {
            //     fcy_assert(0 && "negative value after convolution");
            // }

            T norm_value = ((value - min_value) / (max_value - min_value));
            // spdlog::trace("[{}, {}] norm value: {}", i, j, norm_value);
            norm_mat.At(i, j) = norm_value;
        }
    }

    return norm_mat;
}

template <typename T, typename U>
static Matrix<GSPixel<U>> ConvertRGBImageToGrayScaleMat(const Matrix<Pixel<T>>& image) {
    static_assert(std::is_arithmetic<T>());
    static_assert(std::is_arithmetic<U>());

    spdlog::trace("ConvertRGBImageToGrayScale:");

    Size image_size = image.GetSize();

    Matrix<GSPixel<U>> gspixel_mat{image_size};
    for (size_t i = 0; i < image_size.w; i++) {
        for (size_t j = 0; j < image_size.h; j++) {    // NOTE -V-
            GSPixel<float> gspixel = PixelToGSPixel(Pixel<U>{image.At(i, j)});
            gspixel_mat.At(i, j) = gspixel;
        }     
    }

    return gspixel_mat;
}

template <typename T, typename U>
static void ConvertRGBImageToGrayScaleChanelsMats(const Matrix<Pixel<T>>& rgb_image, 
                                                  Matrix<GSPixel<U>>* red_ch_mat, Matrix<GSPixel<U>>* green_ch_mat, Matrix<GSPixel<U>>* blue_ch_mat) 
{
    static_assert(std::is_arithmetic<T>());
    static_assert(std::is_arithmetic<U>());
    assert(red_ch_mat != nullptr);
    assert(green_ch_mat != nullptr);
    assert(blue_ch_mat != nullptr); 

    Size image_size = rgb_image.GetSize();

    for (size_t i = 0; i < image_size.w; i++) {
        for (size_t j = 0; j < image_size.h; j++) {
            Pixel<T> pixel{rgb_image.At(i, j)};
            red_ch_mat->At(i, j)   = GSPixel<U>{Pixel<U>{pixel}.GetRedColor()};
            green_ch_mat->At(i, j) = GSPixel<U>{Pixel<U>{pixel}.GetGreenColor()};
            blue_ch_mat->At(i, j)  = GSPixel<U>{Pixel<U>{pixel}.GetBlueColor()};
        }
    }
}

template <typename T, typename U>
static void ConvertGrayScaleMatToRGBImage(const Matrix<GSPixel<U>>& gs_image, Matrix<Pixel<T>>* rgb_image) {
    static_assert(std::is_arithmetic<T>());
    static_assert(std::is_arithmetic<U>());
    assert(rgb_image != nullptr);   

    spdlog::trace("ConvertGrayScaleMatToRGBImage");

    Size image_size = gs_image.GetSize();

    for (size_t i = 0; i < image_size.w; i++) {
        for (size_t j = 0; j < image_size.h; j++) {
            rgb_image->At(i, j) = PixelU{GSPixelToPixel(gs_image.At(i, j))};
        }
    }
}

template <typename T, typename U>
static void ConvertGrayScaleChanelsMatsToRGSImage(Matrix<GSPixel<U>>* red_ch_mat, Matrix<GSPixel<U>>* green_ch_mat, Matrix<GSPixel<U>>* blue_ch_mat,
                                                  Matrix<Pixel<T>>* rgb_image) 
{
    static_assert(std::is_arithmetic<T>());
    static_assert(std::is_arithmetic<U>());
    assert(rgb_image != nullptr);   
    assert(red_ch_mat != nullptr);
    assert(green_ch_mat != nullptr);
    assert(blue_ch_mat != nullptr); 

    Size image_size = rgb_image->GetSize();

    for (size_t i = 0; i < image_size.w; i++) {
        for (size_t j = 0; j < image_size.h; j++) {
            Pixel<T> pixel;

            pixel.SetRedColor(red_ch_mat->At(i, j));
            pixel.SetGreenColor(green_ch_mat->At(i, j));
            pixel.SetBlueColor(blue_ch_mat->At(i, j));

            rgb_image->At(i, j) = pixel;
        }
    }   
}

template <typename T>
static T NormalDistributionCurve(T mean_x, T mean_y, T stddev, T x, T y) {
    // normal distribution in case of independent variables and curve itself is symetrical
    // https://en.wikipedia.org/wiki/Multivariate_normal_distribution
    // formula: f(x, y) = (1 / (2 * pi * stddev^2)) * exp(-0.5 * ((x - mean_x)^2 + (y - mean_y)^2) / stddev^2)

    auto sqr = [](T val){ return val * val; };
    T value_pre_exp = T{-0.5} * (sqr(x - mean_x) + sqr(y - mean_y)) / sqr(stddev);
    return (1 / (2 * std::numbers::pi_v<T> * sqr(stddev))) * std::exp(value_pre_exp);
}

} // namespace ftr
