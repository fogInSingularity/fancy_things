#include "pixel.hpp"

Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) noexcept {
    red_ = red;
    green_ = green;
    blue_ = blue;
    alpha_ = alpha;
}

Pixel::Pixel() noexcept {
    red_   = 0;
    green_ = 0; 
    blue_  = 0;
    alpha_ = 0;
}

void Pixel::SetRedColor(uint8_t red)     noexcept { red_ = red; }
void Pixel::SetGreenColor(uint8_t green) noexcept { green_ = green; }
void Pixel::SetBlueColor(uint8_t blue)   noexcept { blue_ = blue; }
void Pixel::SetAlphaColor(uint8_t alpha) noexcept { alpha_ = alpha; }

uint8_t Pixel::GetRedColor()   const noexcept { return red_; }
uint8_t Pixel::GetGreenColor() const noexcept { return green_; }
uint8_t Pixel::GetBlueColor()  const noexcept { return blue_; }
uint8_t Pixel::GetAlphaColor() const noexcept { return alpha_; }

// Pixel operator*(const Pixel pixel_a, const Pixel pixel_b) noexcept {
//     return Pixel(pixel_a.GetRedColor()   * pixel_b.GetRedColor(),
//                  pixel_a.GetGreenColor() * pixel_b.GetGreenColor(),
//                  pixel_a.GetBlueColor()  * pixel_b.GetBlueColor(),
//                  pixel_a.GetAlphaColor() * pixel_b.GetAlphaColor());    
// }

// Pixel Pixel::operator*(const Pixel pixel, const uint8_t scalar) noexcept {
//     return Pixel(pixel.GetRedColor()   * scalar,  
//                  pixel.GetGreenColor() * scalar,
//                  pixel.GetBlueColor()  * scalar,
//                  pixel.GetAlphaColor() * scalar);
// }

// Pixel Pixel::operator+(const Pixel pixel_a, const Pixel pixel_b) noexcept {
//     return Pixel(pixel_a.GetRedColor()   + pixel_b.GetRedColor(),
//                  pixel_a.GetGreenColor() + pixel_b.GetGreenColor(),
//                  pixel_a.GetBlueColor()  + pixel_b.GetBlueColor(),
//                  pixel_a.GetAlphaColor() + pixel_b.GetAlphaColor());
// }

// Pixel operator+(const Pixel pixel, const uint8_t scalar) noexcept {
//     return Pixel(pixel.GetRedColor()   + scalar,  
//                  pixel.GetGreenColor() + scalar,
//                  pixel.GetBlueColor()  + scalar,
//                  pixel.GetAlphaColor() + scalar);
// }
