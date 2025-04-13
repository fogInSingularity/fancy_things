#ifndef THREAD_BRIDGE_HPP_
#define THREAD_BRIDGE_HPP_

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>

#include "helpers/common.hpp"
#include "helpers/trace_calls.hpp"
#include "filter/matrix.hpp"
#include "filter/pixel.hpp"
#include "filter/filters.hpp"
#include "filter/utility.hpp"

namespace brg {

class ThreadBridge {
  private:
    std::mutex mutex_;
    ftr::Matrix<ftr::PixelU> raw_image_;
    std::queue<ftr::FilterTypes> filters_queue_;
    bool is_image_updated_;
    std::condition_variable has_filters_in_queue_;  

    alignas(hlp::kCacheLineSize) std::atomic<bool> is_finished_;
  public:
    explicit ThreadBridge(const ftr::Matrix<ftr::PixelU> raw_image)
        : raw_image_{raw_image}, is_image_updated_{false}, is_finished_{false}
    { 
        hlp::trace_call(); 
    }

    void PushFilter(ftr::FilterTypes filter_type) {
        hlp::trace_call();
        std::unique_lock g{mutex_};

        filters_queue_.push(filter_type);

        has_filters_in_queue_.notify_one();
    }

    // ftr::FilterTypes PopFilter() {
    //     hlp::trace_call();
    //     std::unique_lock g{mutex_};

    //     ftr::FilterTypes filter_type = filters_queue_.front();
    //     filters_queue_.pop();

    //     return filter_type;
    // }

    // bool IsQueueEmpty() {
    //     std::unique_lock g{mutex_};

    //     bool is_empty = filters_queue_.empty();

    //     return is_empty;
    // }

    ftr::FilterTypes WaitOnQueueForFilter() {
        std::unique_lock g{mutex_};

        while (filters_queue_.empty() && !is_finished_) { // prevent spurious wakeup
            has_filters_in_queue_.wait(g);
        }

        if (is_finished_) {
            return ftr::FilterTypes::None;
        }

        auto filter_type = filters_queue_.front();
        filters_queue_.pop();

        return filter_type;
    }

    ftr::Matrix<ftr::PixelU> CurrentImage() {
        hlp::trace_call();
        std::unique_lock g{mutex_};

        ftr::Matrix<ftr::PixelU> mat{raw_image_};

        return mat;
    }

    void UpdateImage(ftr::Matrix<ftr::PixelU> new_image) {
        hlp::trace_call();
        std::unique_lock g{mutex_};

        raw_image_ = new_image;
        is_image_updated_ = true;
    }

    bool IsUpdated() {
        std::unique_lock g{mutex_};

        bool is_updated = is_image_updated_;
        is_image_updated_ = false;

        return is_updated;
    }

    ftr::Size ImageSize() {
        hlp::trace_call();
        std::unique_lock g{mutex_};

        return raw_image_.GetSize();
    }

    bool IsFinished() const {
        // NOTE relaxed because flag only used as flag for termination
        return is_finished_.load(std::memory_order_relaxed);     
    }

    void IsFinished(bool is_finished) {
        hlp::trace_call();
        is_finished_.store(is_finished, std::memory_order_relaxed);

        has_filters_in_queue_.notify_one();
    }
};

void ImageRenderDriver(ThreadBridge* thread_bridge);
void FilterDriver(ThreadBridge* thread_bridge);
void UserChoiceDriver(ThreadBridge* thread_bridge);

} // namespace brg

#endif // THREAD_BRIDGE_HPP_
