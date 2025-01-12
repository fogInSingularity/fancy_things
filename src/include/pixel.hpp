#ifndef PIXEL_HPP_
#define PIXEL_HPP_

#include <cstdint>
#include <type_traits>
#include <algorithm>

#include "spdlog/spdlog.h"

#include <fcy_assert.hpp>

namespace fcy {

template <typename T>
class GSPixel;

template <typename T>
class Pixel {
  // private:
  public: // public becase it doenst have any invariants
    T red_;
    T green_;
    T blue_;
    T alpha_;
  public:
    explicit Pixel(T red = 0, T green = 0, T blue = 0, T alpha = 255) noexcept 
        : red_{red}, green_{green}, blue_{blue}, alpha_{alpha} {}
 
    template <typename U>
    explicit Pixel(const Pixel<U>& pixel) noexcept {
        spdlog::trace("Pixel copy constuctor from Pixel<U>: pixel red: {}, green: {}, blue: {}, alpha: {}", 
                      pixel.red_, pixel.green_, pixel.blue_, pixel.alpha_);

        red_   = static_cast<T>(pixel.red_);
        green_ = static_cast<T>(pixel.green_);
        blue_  = static_cast<T>(pixel.blue_);
        alpha_ = static_cast<T>(pixel.alpha_);
    }

    template <typename U>
    Pixel& operator=(const Pixel<U>& pixel) noexcept {
        spdlog::trace("Pixel copy assigment from Pixel<U>: pixel red: {}, green: {}, blue: {}, alpha: {}", 
                      pixel.red_, pixel.green_, pixel.blue_, pixel.alpha_);

        Pixel<T> new_pixel {
            static_cast<T>(pixel.red_),
            static_cast<T>(pixel.green_),
            static_cast<T>(pixel.blue_),
            static_cast<T>(pixel.alpha_)
        };

        return new_pixel;
    }

    void SetRedColor(T red)     noexcept { red_ = red; }
    void SetGreenColor(T green) noexcept { green_ = green; }
    void SetBlueColor(T blue)   noexcept { blue_ = blue; }
    void SetAlphaColor(T alpha) noexcept { alpha_ = alpha; }

    T GetRedColor()   const noexcept { return red_; }
    T GetGreenColor() const noexcept { return green_; }
    T GetBlueColor()  const noexcept { return blue_; }
    T GetAlphaColor() const noexcept { return alpha_; }

    Pixel& operator+=(Pixel pixel) noexcept {
        red_   += pixel.red_; 
        green_ += pixel.green_;
        blue_  += pixel.blue_;
        alpha_ += pixel.alpha_;

        return *this;
    }

    template <typename U>
    Pixel& operator*=(U scalar) noexcept {
        red_   = static_cast<T>(static_cast<U>(red_)   * scalar);
        green_ = static_cast<T>(static_cast<U>(green_) * scalar); 
        blue_  = static_cast<T>(static_cast<U>(blue_)  * scalar); 
        alpha_ = static_cast<T>(static_cast<U>(alpha_) * scalar);

        return *this;       
    }

    Pixel& operator*=(T scalar) noexcept {
        red_   *= scalar;
        green_ *= scalar; 
        blue_  *= scalar; 
        alpha_ *= scalar; 

        return *this;       
    }

    Pixel& operator*=(Pixel pixel) noexcept {
        red_   *= pixel.red_; 
        green_ *= pixel.green_;
        blue_  *= pixel.blue_;
        alpha_ *= pixel.alpha_;

        return *this;
    }
};

template <typename T>
inline Pixel<T> operator+(Pixel<T> pixel_a, Pixel<T> pixel_b) noexcept {
    return pixel_a += pixel_b;
}

template <typename T, typename U>
inline Pixel<T> operator*(Pixel<T> pixel, U scalar) noexcept {
    return pixel *= scalar;
}

template <typename T>
inline Pixel<T> operator*(Pixel<T> pixel, T scalar) noexcept {
    return pixel *= scalar;
}

template <typename T>
inline Pixel<T> operator*(Pixel<T> pixel_a, Pixel<T> pixel_b) noexcept {
    return pixel_a *= pixel_b;
}

using PixelU = Pixel<uint8_t>;
using PixelF = Pixel<float>;
using PixelD = Pixel<double>;

template <typename T>
class GSPixel {
    static_assert(std::is_arithmetic<T>()); // uint8_t float double
  // private:
  public:
    T gray_;
  public:
    explicit GSPixel(T gray = 0) : gray_{gray} {}

    T GetGray() const { return gray_; }
    void SetGray(T gray) { gray_ = gray; }

    template <typename U>
    GSPixel& operator*=(U scalar) {
        gray_ *= scalar;
        return *this;
    }

    GSPixel& operator+=(GSPixel<T> gspixel) {
        gray_ += gspixel.gray_;
        return *this;
    }
};

template <typename T, typename U>
GSPixel<T> operator*(GSPixel<T> gspixel, U scalar) {
    return gspixel *= scalar;
}

template <typename T>
GSPixel<T> operator+(GSPixel<T> gspixel_a, GSPixel<T> gspixel_b) {
    return gspixel_a += gspixel_b;
}

template <typename T>
Pixel<T> GSPixelToPixel(GSPixel<T> gspixel) {
    if (gspixel.GetGray() < 0) {
        fcy_assert(0);
    }
    spdlog::trace("GSPixel to Pixel: gspixel gray: {}", gspixel.GetGray());

    T clamped_value = std::clamp(gspixel.GetGray(), static_cast<T>(0), static_cast<T>(255));
    spdlog::trace("clamped value: {}", clamped_value);

    T new_rgb_value = clamped_value;
    Pixel<T> pixel{new_rgb_value, new_rgb_value, new_rgb_value};

    return pixel;
}

template <typename T>
GSPixel<T> PixelToGSPixel(Pixel<T> pixel) {
    spdlog::trace("Pixel to GSPixel: pixel red: {}, green: {}, blue: {}", 
                  pixel.GetRedColor(), pixel.GetGreenColor(), pixel.GetBlueColor());
    
    T red = pixel.GetRedColor();
    T green = pixel.GetGreenColor();
    T blue = pixel.GetBlueColor();

    // T new_gray = (red + green + blue) / 3;
    // T new_gray = (std::max({red, green, blue}) + std::min({red, green, blue})) / 2;
    T new_gray = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
    // T new_gray = 0.299 * red + 0.587 * green + 0.114 * blue;
    spdlog::trace("Pixel to GSPixel: result: new gray: {}", new_gray);
    GSPixel<T> gspixel{new_gray};

    return gspixel;
}

} // namespace fcy

#include <spdlog/common.h>

namespace fmt {
template<typename T>
struct formatter<fcy::GSPixel<T>> : fmt::formatter<std::string>
{
    auto format(fcy::GSPixel<T> my, format_context &ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "[gspixel.gray = {}]", my.GetGray());
    }
};
} // namespace fmt

#endif // PIXEL_HPP_
