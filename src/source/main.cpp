#include <cstdlib>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "fancy_things.hpp"

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
    spdlog::set_level(spdlog::level::debug);
#endif // NDEBUG
    
    // log argv
    for (int i = 0; i < argc; i++) {
        spdlog::info("argv[{}]: {}", i, argv[i]);
    }

    if (argc < 2) {
        spdlog::error("0 files had been passed");
        std::cerr << "0 files had been passed" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Image image;
    bool is_load_successful = image.loadFromFile(std::string(argv[1]));
    if (!is_load_successful) {
        spdlog::error("Cant load image({})", argv[1]);
        std::cerr << "Cant load image(" << std::string(argv[1]) << ")" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Vector2u image_size = image.getSize();

    sf::RenderWindow window(sf::VideoMode(image_size.x, 
                                          image_size.y), 
                            fcy::WindowName);

    fcy::RenderState render_state = {
        .use_algo = false,
        .save_image = false,
    };

    while (window.isOpen()) {
        fcy::MainLoop(&window, &image, &render_state);
    }

    return EXIT_SUCCESS;
}
