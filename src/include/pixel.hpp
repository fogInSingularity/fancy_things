#ifndef PIXEL_HPP_
#define PIXEL_HPP_

#include <cstdint>

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
    Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) noexcept {
        red_ = red;
        green_ = green;
        blue_ = blue;
        alpha_ = alpha;
    }

    Pixel() noexcept {
        red_   = 0;
        green_ = 0; 
        blue_  = 0;
        alpha_ = 0;
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

    Pixel& operator*=(uint8_t scalar) noexcept {
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

inline Pixel operator*(Pixel pixel_a, Pixel pixel_b) noexcept {
    return pixel_a *= pixel_b;
}

#endif // PIXEL_HPP_
