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
    Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) noexcept;
    Pixel() noexcept;
    // ~Pixel() = default;
        
    void SetRedColor(uint8_t red) noexcept;
    void SetGreenColor(uint8_t green) noexcept;
    void SetBlueColor(uint8_t blue) noexcept;
    void SetAlphaColor(uint8_t alpha) noexcept; 

    uint8_t GetRedColor() const noexcept;
    uint8_t GetGreenColor() const noexcept;
    uint8_t GetBlueColor() const noexcept;
    uint8_t GetAlphaColor() const noexcept;

    // Pixel operator*(const Pixel pixel, const uint8_t scalar) noexcept; 
    // Pixel operator+(const Pixel pixel_a, const Pixel pixel_b) noexcept; 
    // Pixel operator+(const Pixel pixel, const uint8_t scalar) noexcept;
    
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
