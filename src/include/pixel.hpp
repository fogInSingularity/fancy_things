#ifndef PIXEL_HPP_
#define PIXEL_HPP_

#include <cstdint>

namespace fcy {

template <typename T>
class PixelT {
  // private:
  public: // public becase it doenst have any invariants
    T red_;
    T green_;
    T blue_;
    T alpha_;
  public:
    explicit PixelT(T red = 0, T green = 0, T blue = 0, T alpha = 0) noexcept 
        : red_{red}, green_{green}, blue_{blue}, alpha_{alpha} {}
 
    template <typename U>
    explicit PixelT(const PixelT<U>& pixel) noexcept {
        red_   = static_cast<T>(pixel.red_);
        green_ = static_cast<T>(pixel.green_);
        blue_  = static_cast<T>(pixel.blue_);
        alpha_ = static_cast<T>(pixel.alpha_);
    }

    void SetRedColor(T red)     noexcept { red_ = red; }
    void SetGreenColor(T green) noexcept { green_ = green; }
    void SetBlueColor(T blue)   noexcept { blue_ = blue; }
    void SetAlphaColor(T alpha) noexcept { alpha_ = alpha; }

    T GetRedColor()   const noexcept { return red_; }
    T GetGreenColor() const noexcept { return green_; }
    T GetBlueColor()  const noexcept { return blue_; }
    T GetAlphaColor() const noexcept { return alpha_; }

    PixelT& operator+=(PixelT pixel) noexcept {
        red_   += pixel.red_; 
        green_ += pixel.green_;
        blue_  += pixel.blue_;
        alpha_ += pixel.alpha_;

        return *this;
    }

    template <typename U>
    PixelT& operator*=(U scalar) noexcept {
        red_   = static_cast<T>(static_cast<U>(red_)   * scalar);
        green_ = static_cast<T>(static_cast<U>(green_) * scalar); 
        blue_  = static_cast<T>(static_cast<U>(blue_)  * scalar); 
        alpha_ = static_cast<T>(static_cast<U>(alpha_) * scalar); // alpha stays the same

        return *this;       
    }

    PixelT& operator*=(T scalar) noexcept {
        red_   *= scalar;
        green_ *= scalar; 
        blue_  *= scalar; 
        alpha_ *= scalar; // alpha stays the same

        return *this;       
    }

    PixelT& operator*=(PixelT pixel) noexcept {
        red_   *= pixel.red_; 
        green_ *= pixel.green_;
        blue_  *= pixel.blue_;
        alpha_ *= pixel.alpha_;

        return *this;
    }
};

template <typename T>
inline PixelT<T> operator+(PixelT<T> pixel_a, PixelT<T> pixel_b) noexcept {
    return pixel_a += pixel_b;
}

template <typename T, typename U>
inline PixelT<T> operator*(PixelT<T> pixel, U scalar) noexcept {
    return pixel *= scalar;
}

template <typename T>
inline PixelT<T> operator*(PixelT<T> pixel, T scalar) noexcept {
    return pixel *= scalar;
}

template <typename T>
inline PixelT<T> operator*(PixelT<T> pixel_a, PixelT<T> pixel_b) noexcept {
    return pixel_a *= pixel_b;
}

using Pixel = PixelT<uint8_t>;

} // namespace fcy

#endif // PIXEL_HPP_
