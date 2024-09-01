#ifndef FILTERS_HPP_
#define FILTERS_HPP_

#include <stdint.h>
#include <stddef.h>

namespace Filters { 
    struct Pixel {
        union {
            uint8_t red;
            uint8_t x;
        };
        union {
            uint8_t green;
            uint8_t y;
        };
        union {
            uint8_t blue;
            uint8_t z;
        };
        union {
            uint8_t alpha;
            uint8_t w;
        };
    };

    void ReversFilter(Pixel* image, const size_t image_size_x, const size_t image_size_y);
    void ThresholdFilter(Pixel* image, const size_t image_size_x, const size_t image_size_y);
    void GaussianBlur(Pixel* image, const size_t image_size_x, const size_t image_size_y);
} // namespace Filters

#endif // FILTERS_HPP_
