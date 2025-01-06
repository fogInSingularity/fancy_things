#ifndef FILTERS_HPP_
#define FILTERS_HPP_

#include <cstddef>

#include "pixel.hpp"

namespace Filters { 
    void ReversFilter(Pixel* image, const size_t image_size_x, const size_t image_size_y);
    void ThresholdFilter(Pixel* image, const size_t image_size_x, const size_t image_size_y);
    void GaussianBlur(Pixel* image, const size_t image_size_x, const size_t image_size_y);
} // namespace Filters

#endif // FILTERS_HPP_
