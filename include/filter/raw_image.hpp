#ifndef RAW_IMAGE_HPP_
#define RAW_IMAGE_HPP_

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <cstring>

#include <spdlog/spdlog.h>

#include "filter/pixel.hpp"
#include "filter/filters.hpp"
#include "filter/utility.hpp"

namespace ftr {

class RawImage {
  private:
    ftr::Size size_;
    PixelU* pixel_buf_;
  public:
    RawImage(const uint8_t* raw_image_buf, Size size) 
        : size_{size}, pixel_buf_{new PixelU[size.w * size.h]}
    {
        assert(raw_image_buf != nullptr);

        spdlog::trace("RawImage constructor call: {:p} {}x{}", reinterpret_cast<const void*>(raw_image_buf), size_.w, size_.h);

        size_t area = size.w * size.h;
        pixel_buf_ = new PixelU[area];
        const PixelU* image_buf = reinterpret_cast<const PixelU*>(raw_image_buf);
        std::copy(image_buf, image_buf + area, pixel_buf_);
    }

    ~RawImage() {
        delete[] pixel_buf_; 
    }

    const uint8_t* GetPixelPtr() {
        return  reinterpret_cast<const uint8_t*>(pixel_buf_);
    }

    size_t GetWidth()  const { return size_.w; }
    size_t GetHeight() const { return size_.h; }

    void Filter(IFilter* filter) {
        // (*filter)(pixel_buf_, size_.w, size_.h);
    }
};

} // namespace ftr
 
#endif // RAW_IMAGE_HPP_
