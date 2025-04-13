#include "bridge/thread_bridge.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

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

    thread_bridge->IsFinished(true);

    spdlog::debug("ImageRenderDriver finished");
}

void FilterDriver(ThreadBridge* thread_bridge) {
    hlp::trace_call();

    while (!thread_bridge->IsFinished()) {
        // if (!thread_bridge->IsQueueEmpty()) {
        //     spdlog::debug("Filter in queue found");
        //     auto filter_type = thread_bridge->PopFilter();

        //     std::unique_ptr<ftr::IFilter> filter = ftr::ProduceFilter(filter_type);
        //     auto mat = thread_bridge->CurrentImage();
        //     (*filter)(&mat);

        //     thread_bridge->UpdateImage(mat);
        //     spdlog::debug("Image updated");
        // }
        
        auto filter_type = thread_bridge->WaitOnQueueForFilter();        
        if (filter_type == ftr::FilterTypes::None) {
            break;
        }

        std::unique_ptr<ftr::IFilter> filter = ftr::ProduceFilter(filter_type);
        auto mat = thread_bridge->CurrentImage();
        (*filter)(&mat);

        thread_bridge->UpdateImage(mat);
        spdlog::debug("Image updated");
    }

    spdlog::debug("FilterDriver finished");
}

void UserChoiceDriver(ThreadBridge* thread_bridge) {
    hlp::trace_call();

    std::cout << "Enter number of 'q' to exit" << std::endl;
    for (int i = 0; i < hlp::FromEnum(ftr::FilterTypes::CountOfFilters); i++) {
        std::cout << "[" << i << "] " << ftr::FilterTypesToStr(hlp::ToEnum<ftr::FilterTypes>(i)) << std::endl;
    }

    while (!thread_bridge->IsFinished()) {
        std::string input_str;
        std::getline(std::cin, input_str);
        std::cout << "\033[1A\033[2K"; // clear
        
        if (input_str == "q") {
            break;
        }

        int filter_num = 0;
        try {
            filter_num = std::stoi(input_str);
        } catch (const std::invalid_argument& e) {
            std::cout << "Enter number!" << std::endl;
            break;
        } catch (const std::out_of_range& e) {
            std::cout << "Number is too big!" << std::endl;
            break;
        } catch (...) {
            std::cout << "Unexpected error!" << std::endl;
            break;
        }
         
        spdlog::debug("number entered: {}", filter_num);

        if (filter_num <= hlp::FromEnum(ftr::FilterTypes::None) || filter_num > hlp::FromEnum(ftr::FilterTypes::CountOfFilters)) {
            break;
        }

        ftr::FilterTypes filter_type = hlp::ToEnum<ftr::FilterTypes>(filter_num);
        thread_bridge->PushFilter(filter_type);
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    thread_bridge->IsFinished(true);

    spdlog::debug("UserChoiceDriver finished");
}

} // namespace brg
