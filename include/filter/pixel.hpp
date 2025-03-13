#ifndef PIXEL_HPP_
#define PIXEL_HPP_

#include <cassert>
#include <cstdint>
#include <algorithm>

#include <spdlog/spdlog.h>

namespace ftr {

template <typename T>
class Pixel {
  // private:
  public: // public becase it doenst have any invariants
    T red_   = 0;
    T green_ = 0;
    T blue_  = 0;
    T alpha_ = 255;
  public:
    Pixel() noexcept {}
    Pixel(T red, T green, T blue, T alpha = 255) noexcept
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

    Pixel(const Pixel& pixel) noexcept {
        spdlog::trace("Pixel copy constuctor from Pixel<U>: pixel red: {}, green: {}, blue: {}, alpha: {}", 
                      pixel.red_, pixel.green_, pixel.blue_, pixel.alpha_);
        red_ = pixel.red_;
        green_ = pixel.green_;
        blue_ = pixel.blue_;
        alpha_ = pixel.alpha_;
   }

    template <typename U>
    Pixel& operator=(const Pixel<U>& pixel) noexcept {
        spdlog::trace("Pixel copy assigment from Pixel<U>: pixel red: {}, green: {}, blue: {}, alpha: {}", 
                      pixel.red_, pixel.green_, pixel.blue_, pixel.alpha_);

        red_   = static_cast<T>(pixel.red_);
        green_ = static_cast<T>(pixel.green_);
        blue_  = static_cast<T>(pixel.blue_);
        alpha_ = static_cast<T>(pixel.alpha_);

        return *this;
    }

    Pixel& operator=(const Pixel& pixel) noexcept {
        spdlog::trace("Pixel copy assigment from Pixel<U>: pixel red: {}, green: {}, blue: {}, alpha: {}", 
                      pixel.red_, pixel.green_, pixel.blue_, pixel.alpha_);

        red_   = pixel.red_;
        green_ = pixel.green_;
        blue_  = pixel.blue_;
        alpha_ = pixel.alpha_;

        return *this;
    }

    void SetRedColor(T red)     noexcept { red_   = red; }
    void SetGreenColor(T green) noexcept { green_ = green; }
    void SetBlueColor(T blue)   noexcept { blue_  = blue; }
    void SetAlphaColor(T alpha) noexcept { alpha_ = alpha; }

    T GetRedColor()   const noexcept { return red_; }
    T GetGreenColor() const noexcept { return green_; }
    T GetBlueColor()  const noexcept { return blue_; }
    T GetAlphaColor() const noexcept { return alpha_; }

    Pixel& operator+=(Pixel pixel) noexcept {
        red_   += pixel.red_; 
        green_ += pixel.green_;
        blue_  += pixel.blue_;
        // alpha_ += pixel.alpha_;

        return *this;
    }

    Pixel& operator-=(Pixel pixel) noexcept {
        red_   -= pixel.red_; 
        green_ -= pixel.green_;
        blue_  -= pixel.blue_;
        // alpha_ -= pixel.alpha_;

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
Pixel<T> operator+(Pixel<T> pixel_a, Pixel<T> pixel_b) noexcept {
    return pixel_a += pixel_b;
}

template <typename T>
Pixel<T> operator-(Pixel<T> pixel_a, Pixel<T> pixel_b) noexcept {
    return pixel_a -= pixel_b;
}

template <typename T, typename U>
Pixel<T> operator*(Pixel<T> pixel, U scalar) noexcept {
    return pixel *= scalar;
}

template <typename T>
Pixel<T> operator*(Pixel<T> pixel, T scalar) noexcept {
    return pixel *= scalar;
}

template <typename T>
Pixel<T> operator*(Pixel<T> pixel_a, Pixel<T> pixel_b) noexcept {
    return pixel_a *= pixel_b;
}

using PixelU = Pixel<uint8_t>;
using PixelF = Pixel<float>;
using PixelD = Pixel<double>;

template <typename T>
using GSPixel = T;

using GSPixelF = GSPixel<float>;
using GSPixelD = GSPixel<double>;

template <typename T>
Pixel<T> GSPixelToPixel(GSPixel<T> gspixel) {
    if (gspixel < 0) { assert(0 && "gray < 0"); }

    spdlog::trace("GSPixel to Pixel: gspixel gray: {}", gspixel);

    T clamped_value = std::clamp(gspixel, static_cast<T>(0), static_cast<T>(1)) * 255;
    // spdlog::trace("clamped value: {}", clamped_value);

    T new_rgb_value = clamped_value;
    // T new_rgb_value = gspixel * 255;
    Pixel<T> pixel{new_rgb_value, new_rgb_value, new_rgb_value};

    return pixel;
}

template <typename T>
GSPixel<T> PixelToGSPixel(Pixel<T> pixel) {
    spdlog::trace("Pixel to GSPixel: pixel red: {}, green: {}, blue: {}", 
                  pixel.GetRedColor(), pixel.GetGreenColor(), pixel.GetBlueColor());
    
    T red   = pixel.GetRedColor();
    T green = pixel.GetGreenColor();
    T blue  = pixel.GetBlueColor();

    // T new_gray = (red + green + blue) / 3; 
    // T new_gray = (std::max({red, green, blue}) + std::min({red, green, blue})) / 2;
    T new_gray = (0.2126 * red + 0.7152 * green + 0.0722 * blue) / 255.0; // best way
    // T new_gray = 0.299 * red + 0.587 * green + 0.114 * blue;
    spdlog::trace("Pixel to GSPixel: result: new gray: {}", new_gray);

    return new_gray;
}

} // namespace ftr

#endif // PIXEL_HPP_
