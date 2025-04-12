#include "bridge/thread_bridge.hpp"

#include <iostream>

#include <memory>
#include <spdlog/spdlog.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>

#include "helpers/common.hpp"
#include "helpers/trace_calls.hpp"
#include "filter/filters.hpp"
#include "filter/utility.hpp"
#include "fcy_ui/fancy_things.hpp"

namespace brg {

void ImageRenderDriver(ThreadBridge* thread_bridge) {
    hlp::trace_call();   
    
    fcy::MainLoop(thread_bridge);

    spdlog::debug("ImageRenderDriver finished");
}

void FilterDriver(ThreadBridge* thread_bridge) {
    hlp::trace_call();

    while (!thread_bridge->IsFinished()) {
        if (!thread_bridge->IsQueueEmpty()) {
            spdlog::debug("Filter in queue found");
            auto filter_type = thread_bridge->PopFilter();

            std::unique_ptr<ftr::IFilter> filter = ftr::ProduceFilter(filter_type);
            auto mat = thread_bridge->CurrentImage();
            (*filter)(&mat);

            thread_bridge->UpdateImage(mat);
            spdlog::debug("Image updated");
        }
    }

    spdlog::debug("FilterDriver finished");
}

void UserChoiceDriver(ThreadBridge* thread_bridge) {
    hlp::trace_call();

    for (int i = 0; i < FromEnum(ftr::FilterTypes::CountOfFilters); i++) {
        std::cout << "[" << i << "] " << ftr::FilterTypesToStr(ToEnum<ftr::FilterTypes>(i)) << std::endl;
    }

    while (!thread_bridge->IsFinished()) {
        int in = 0;
        std::cin >> in;
        spdlog::debug("number entered: {}", in);

        if (in <= FromEnum(ftr::FilterTypes::None) || in > FromEnum(ftr::FilterTypes::CountOfFilters)) {
            break;
        }

        ftr::FilterTypes filter_type = ToEnum<ftr::FilterTypes>(in);
        thread_bridge->PushFilter(filter_type);
    }

    spdlog::debug("UserChoiceDriver finished");
}

} // namespace brg
