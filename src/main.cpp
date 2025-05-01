#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "filter/matrix.hpp"
#include "filter/pixel.hpp"
#include "bridge/thread_bridge.hpp"
#include "helpers/common.hpp"

// распаралеллить 
// сделать 

int main(const int argc, const char* argv[]) {
    // use when spdlog v2
    // auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("fancy_things.log", true);
    // auto logger = std::make_shared<spdlog::logger>("fancy_things", file_sink);
    // spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%@] %v");
    // spdlog::set_global_logger(logger);

    auto logger = spdlog::basic_logger_mt("fancy_things", "fancy_things.log", true);
    spdlog::set_default_logger(logger);

#if defined (NDEBUG)
    spdlog::set_level(spdlog::level::info);
#else // NDEBUG
    spdlog::flush_on(spdlog::level::trace);
    spdlog::set_level(spdlog::level::trace);
#endif // NDEBUG
 
    spdlog::info("program pid: {}", hlp::GetPid());
    // log argv
    for (int i = 0; i < argc; i++) {
        spdlog::info("argv[{}]: {}", i, argv[i]);
    }

    if (argc < 2) {
        spdlog::error("0 files had been passed");
        std::cerr << "0 files had been passed" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Image image{};
    bool is_load_successful = image.loadFromFile(std::string(argv[1]));
    if (!is_load_successful) {
        spdlog::error("Cant load image({})", argv[1]);
        std::cerr << "Cant load image(" << std::string(argv[1]) << ")" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Vector2u image_size = image.getSize();
    
    ftr::Matrix<ftr::PixelU> raw_image{
        {image_size.x, image_size.y}, 
        reinterpret_cast<const ftr::PixelU*>(image.getPixelsPtr())
    };

    brg::ThreadBridge thread_bridge{raw_image};

    // create thread with ui and filters
    std::thread image_render_thread{brg::ImageRenderDriver, &thread_bridge};
    std::thread filter_thread{brg::FilterDriver, &thread_bridge};
    std::thread user_choice_thread{brg::UserChoiceDriver, &thread_bridge};

    image_render_thread.join();
    filter_thread.join();
    user_choice_thread.join();

    spdlog::info("Exiting");

    return EXIT_SUCCESS;
}
