#include "filters.hpp"

#include <stdint.h>
#include <stddef.h>
#include <cassert>
#include <climits>
//#include <algorithm>
#include <cstring>

#include "logging.h"

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

void Filters::ReversFilter(uint8_t* raw_image, const size_t raw_image_size) {
    assert(raw_image != nullptr);
    assert(raw_image_size % sizeof(Pixel) == 0);
 
    Log("ReversFilter call\n");
    
    Pixel pixel = {};
    static_assert(sizeof(Pixel) == 4, "cringe");

    for (uint8_t* iter_pixels = raw_image; iter_pixels < raw_image + raw_image_size; iter_pixels += sizeof(pixel)) {
       // std::copy(iter_pixels, iter_pixels + sizeof(Pixel), &pixel);
        memcpy(&pixel, iter_pixels, sizeof(pixel));
        pixel.red   = UCHAR_MAX - pixel.red;
        pixel.green = UCHAR_MAX - pixel.green;
        pixel.blue  = UCHAR_MAX - pixel.blue;
        // std::copy(&pixel, &pixel + sizeof(Pixel), iter_pixels); 
        memcpy(iter_pixels, &pixel, sizeof(pixel));
    }
}

void Filters::ThresholdFilter(uint8_t* raw_image, const size_t raw_image_size) {
    assert(raw_image != nullptr);
    assert(raw_image_size % 4 == 0);

    Log("ThresholdFilter call\n");
}
