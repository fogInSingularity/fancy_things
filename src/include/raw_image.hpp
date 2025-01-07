#ifndef RAW_IMAGE_HPP_
#define RAW_IMAGE_HPP_

#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <cstring>

#include "fcy_assert.hpp"
#include "pixel.hpp"
#include "filters.hpp"
#include "spdlog/spdlog.h"

namespace fcy {

class RawImage {
  private:
    Pixel* pixel_buf_;
    size_t width_;
    size_t height_;
  public:
    RawImage(const uint8_t* raw_image_buf, size_t width, size_t height) {
        fcy_assert(raw_image_buf != nullptr);

        spdlog::trace("RawImage constructor call: {:p} {}x{}", reinterpret_cast<const void*>(raw_image_buf), width, height);

        pixel_buf_ = new Pixel[width * height];
        // std::copy(raw_image_buf, raw_image_buf + width * height, pixel_buf_);
        std::memcpy(pixel_buf_, raw_image_buf, width * height * sizeof(Pixel));
        width_ = width;
        height_ = height;
    }

    ~RawImage() {
        delete[] pixel_buf_; 
    }

    const uint8_t* GetPixelPtr() {
        return  reinterpret_cast<const uint8_t*>(pixel_buf_);
    }

    void Filter(IFilter* filter) {
        (*filter)(pixel_buf_, width_, height_);
    }
};

} // namespace fcy
 
#endif // RAW_IMAGE_HPP_
