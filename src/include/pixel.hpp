#ifndef PIXEL_HPP_
#define PIXEL_HPP_

#include <cstdint>

namespace fcy {

class Pixel {
  private:
    union {
        uint8_t red_;
        uint8_t x_;
    };
    union {
        uint8_t green_;
        uint8_t y_;
    };
    union {
        uint8_t blue_;
        uint8_t z_;
    };
    union {
        uint8_t alpha_;
        uint8_t w_;
    };
  public:
    explicit Pixel(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 0) noexcept {
        red_ = red;
        green_ = green;
        blue_ = blue;
        alpha_ = alpha;
    }
        
    void SetRedColor(uint8_t red)     noexcept { red_ = red; }
    void SetGreenColor(uint8_t green) noexcept { green_ = green; }
    void SetBlueColor(uint8_t blue)   noexcept { blue_ = blue; }
    void SetAlphaColor(uint8_t alpha) noexcept { alpha_ = alpha; }

    uint8_t GetRedColor()   const noexcept { return red_; }
    uint8_t GetGreenColor() const noexcept { return green_; }
    uint8_t GetBlueColor()  const noexcept { return blue_; }
    uint8_t GetAlphaColor() const noexcept { return alpha_; }

    Pixel& operator+=(Pixel pixel) noexcept {
        red_   += pixel.red_; 
        green_ += pixel.green_;
        blue_  += pixel.blue_;
        alpha_ += pixel.alpha_;

        return *this;
    }

    template <typename T>
    Pixel& operator*=(T scalar) noexcept {
        red_   = static_cast<uint8_t>(static_cast<T>(red_)   * scalar);
        green_ = static_cast<uint8_t>(static_cast<T>(green_) * scalar); 
        blue_  = static_cast<uint8_t>(static_cast<T>(blue_)  * scalar); 
        // alpha_ *= scalar; // alpha stays the same

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

inline Pixel operator+(Pixel pixel_a, Pixel pixel_b) noexcept {
    return pixel_a += pixel_b;
}

template <typename T>
inline Pixel operator*(Pixel pixel, T scalar) noexcept {
    return pixel *= scalar;
}

inline Pixel operator*(Pixel pixel_a, Pixel pixel_b) noexcept {
    return pixel_a *= pixel_b;
}

} // namespace fcy

#endif // PIXEL_HPP_
