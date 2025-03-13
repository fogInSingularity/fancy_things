#ifndef FILTERS_HPP_
#define FILTERS_HPP_

#include <cstddef>

#include "filter/pixel.hpp"
#include "filter/matrix.hpp"
#include "filter/utility.hpp"

namespace ftr { 

class IFilter {
  public:
    virtual ~IFilter() {};
    virtual void operator()(Matrix<PixelU>* image) = 0;
};

class ReverseFilter : public IFilter {
  public:
    ~ReverseFilter() override {};
    void operator()(Matrix<PixelU>* image) override;
};

class BoxBlurFilter : public IFilter {
  private:
    Size ker_size_;
  public:
    explicit BoxBlurFilter(Size size = {5, 5}) : ker_size_{size} {}

    ~BoxBlurFilter() override {};
    void operator()(Matrix<PixelU>* image) override;
};

class GaussianBlurFilter : public IFilter {
  private:
    Size ker_size_;
    float mean_;
    float stddev_;
   public:
    explicit GaussianBlurFilter(Size ker_size = {3, 3}, float mean = 0, float stddev = 0.5) 
        : ker_size_{ker_size}, mean_{mean}, stddev_{stddev} {}

    ~GaussianBlurFilter() override {};
    void operator()(Matrix<PixelU>* image) override;
};

class MotionBlurFilter : public IFilter {
  private:
    Size ker_size_;
   public:
    explicit MotionBlurFilter(Size ker_size = {15, 15}) 
        : ker_size_{ker_size} {}

    ~MotionBlurFilter() override {};
    void operator()(Matrix<PixelU>* image) override;
};

class ThresholdFilter : public IFilter {
  private:
    Size ker_size_;
    float threshold_;
  public:
    explicit ThresholdFilter(Size ker_size = {3, 3}, float threashold = 0.16) 
        : ker_size_{ker_size}, threshold_{threashold} {}

    ~ThresholdFilter() override {};
    void operator()(Matrix<PixelU>* image) override;
};

class EmbossingFilter : public IFilter {
  public:
    ~EmbossingFilter() override {};
    void operator()(Matrix<PixelU>* image) override;
};

class EdgeDetectorSobelFilter : public IFilter {
  public:
    ~EdgeDetectorSobelFilter() override {};
    void operator()(Matrix<PixelU>* image) override;
};

class EdgeDetectorLaplacianFilter : public IFilter {
  public:
    ~EdgeDetectorLaplacianFilter() override {};
    void operator()(Matrix<PixelU>* image) override;
};

} // namespace ftr

#endif // FILTERS_HPP_
