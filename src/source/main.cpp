#include <cstdlib>
#include <iostream>
#include <cassert>

#include <SFML/Graphics.hpp>

#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "fancy_things.hpp"

int main(const int argc, const char* argv[]) {
    auto logger = spdlog::basic_logger_mt("fancy_things", "fancy_things.log", true);
    spdlog::set_default_logger(logger);

#if defined (NDEBUG)
    spdlog::set_level(spdlog::level::info);
#else // NDEBUG
    spdlog::flush_on(spdlog::level::trace);
    spdlog::set_level(spdlog::level::debug);
#endif // NDEBUG

    if (argc < 2) {
        spdlog::error("0 files had been passed\n");
        std::cerr << "0 files had been passed" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Image image;
    bool is_load_successful = image.loadFromFile(std::string(argv[1]));
    if (!is_load_successful) {
        spdlog::error("file load wherent successful\n");
        std::cerr << "file load wherent successful" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Vector2u image_size = image.getSize();

    sf::RenderWindow window(sf::VideoMode(image_size.x, 
                                          image_size.y), 
                            FancyThings::WindowName);

    FancyThings::RenderState render_state = {.use_algo = false};

    while (window.isOpen()) {
        FancyThings::MainLoop(&window, &image, &render_state);
    }

    return EXIT_SUCCESS;
}
