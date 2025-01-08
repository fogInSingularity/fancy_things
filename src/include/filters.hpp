#ifndef FILTERS_HPP_
#define FILTERS_HPP_

#include <cstddef>

#include "pixel.hpp"

namespace fcy { 

class IFilter {
  public:
    virtual ~IFilter() {};
    virtual void operator()(Pixel* pixel_buf, size_t width, size_t height) = 0;
};

class ReverseFilter : public IFilter {
  public:
    ~ReverseFilter() override {};
    void operator()(Pixel* pixel_buf, size_t width, size_t height) override;
};

class BoxBlurFilter : public IFilter {
  public:
    ~BoxBlurFilter() override {};
    void operator()(Pixel* pixel_buf, size_t width, size_t height) override;
};

class ThresholdFilter : public IFilter {
  public:
    ~ThresholdFilter() override {};
    void operator()(Pixel* pixel_buf, size_t width, size_t height) override;
};

} // namespace fcy

#endif // FILTERS_HPP_
