#include "filters.hpp"

#include <stdint.h>
#include <stddef.h>
#include <cassert>
#include <climits>
#include <cstring>

#include "logging.h"

// static -----------------------------------------------------------------------------------------



// Filters ----------------------------------------------------------------------------------------

void Filters::ReversFilter(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    assert(image != nullptr);
 
    LogFunctionEntry();

    for (Pixel* iter_pixels = image; iter_pixels < image + image_size_x * image_size_y; iter_pixels++) {
        iter_pixels->red = UCHAR_MAX - iter_pixels->red;
        iter_pixels->green = UCHAR_MAX - iter_pixels->green;
        iter_pixels->blue = UCHAR_MAX - iter_pixels->blue;
    }
}

void Filters::ThresholdFilter(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    assert(image != nullptr);

    LogFunctionEntry();
}

void Filters::GaussianBlur(Pixel* image, const size_t image_size_x, const size_t image_size_y) {
    assert(image != nullptr);

    LogFunctionEntry();

    
}

// static -----------------------------------------------------------------------------------------


