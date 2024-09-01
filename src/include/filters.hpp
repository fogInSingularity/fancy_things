#ifndef FILTERS_HPP_
#define FILTERS_HPP_

#include <stdint.h>
#include <stddef.h>

namespace Filters {
    void ReversFilter(uint8_t* raw_image, const size_t raw_image_size);
    void ThresholdFilter(uint8_t* raw_image, const size_t raw_image_size);
}

#endif // FILTERS_HPP_