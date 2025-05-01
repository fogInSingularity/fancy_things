#ifndef FILTERS_HPP_
#define FILTERS_HPP_

#include <string_view>
#include <memory>

#include "filter/pixel.hpp"
#include "filter/matrix.hpp"
#include "filter/utility.hpp"

namespace ftr { 

enum class FilterTypes {
    None = -1,

    Reverse,
    BoxBlur,
    GaussianBlur,
    MotionBlur,
    Threshold,
    Embossing,
    EdgeDetectorSobel,
    EdgeDetectorLaplacian,

    CountOfFilters, // 8
};

constexpr inline std::string_view FilterTypesToStr(FilterTypes filter_type) {
    using enum FilterTypes;

    switch (filter_type) {
        case Reverse: return "reverse";
        case BoxBlur: return "box blur";
        case GaussianBlur: return "Gaussian blur";
        case MotionBlur: return "motion blur";
        case Threshold: return "threshold";
        case Embossing: return "embossing";
        case EdgeDetectorSobel: return "edge detector(Sobel)";
        case EdgeDetectorLaplacian: return "edge detector(Laplacian)";          
        case CountOfFilters: 
        default:
            return "<invalid filter type>";
    }
}

class IFilter {
  public:
    virtual ~IFilter() = default;
    virtual void operator()(Matrix<PixelU>* image) = 0;
};

class ReverseFilter : public IFilter {
  public:
    ~ReverseFilter() override = default;
    void operator()(Matrix<PixelU>* image) override;
};

class BoxBlurFilter : public IFilter {
  private:
    Size ker_size_;
  public:
    explicit BoxBlurFilter(Size size = {5, 5}) : ker_size_{size} {}

    ~BoxBlurFilter() override = default;
    void operator()(Matrix<PixelU>* image) override;
};

class GaussianBlurFilter : public IFilter {
  private:
    Size ker_size_;
    float mean_;
    float stddev_;
   public:
    explicit GaussianBlurFilter(Size ker_size = {3, 3}, float mean = 0.0f, float stddev = 0.5f) 
        : ker_size_{ker_size}, mean_{mean}, stddev_{stddev} {}

    ~GaussianBlurFilter() override = default;
    void operator()(Matrix<PixelU>* image) override;
};

class MotionBlurFilter : public IFilter {
  private:
    Size ker_size_;
   public:
    explicit MotionBlurFilter(Size ker_size = {15, 15}) 
        : ker_size_{ker_size} {}

    ~MotionBlurFilter() override = default;
    void operator()(Matrix<PixelU>* image) override;
};

class ThresholdFilter : public IFilter {
  private:
    Size ker_size_;
    float threshold_;
  public:
    explicit ThresholdFilter(Size ker_size = {3, 3}, float threshold = 0.16f) 
        : ker_size_{ker_size}, threshold_{threshold} {}

    ~ThresholdFilter() override = default;
    void operator()(Matrix<PixelU>* image) override;
};

class EmbossingFilter : public IFilter {
  public:
    ~EmbossingFilter() override = default;
    void operator()(Matrix<PixelU>* image) override;
};

class EdgeDetectorSobelFilter : public IFilter {
  public:
    ~EdgeDetectorSobelFilter() override = default;
    void operator()(Matrix<PixelU>* image) override;
};

class EdgeDetectorLaplacianFilter : public IFilter {
  public:
    ~EdgeDetectorLaplacianFilter() override = default;
    void operator()(Matrix<PixelU>* image) override;
};

class IdentityFilter : public IFilter {
  public:
    ~IdentityFilter() override = default;
    void operator()([[maybe_unused]] Matrix<PixelU>* image) override {
        // do nothing
    };
};

inline std::unique_ptr<IFilter> ProduceFilter(FilterTypes filter_type) {
    using enum FilterTypes;

    switch (filter_type) {
        case Reverse: return std::make_unique<ReverseFilter>();
        case BoxBlur: return std::make_unique<BoxBlurFilter>();
        case GaussianBlur: return std::make_unique<GaussianBlurFilter>();
        case MotionBlur: return std::make_unique<MotionBlurFilter>();
        case Threshold: return std::make_unique<ThresholdFilter>();
        case Embossing: return std::make_unique<EmbossingFilter>();
        case EdgeDetectorSobel: return std::make_unique<EdgeDetectorSobelFilter>();
        case EdgeDetectorLaplacian: return std::make_unique<EdgeDetectorLaplacianFilter>();
        case CountOfFilters: 
        default:
            return std::make_unique<IdentityFilter>();
    }
}

} // namespace ftr

#endif // FILTERS_HPP_
