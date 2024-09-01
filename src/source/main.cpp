#include <cstdlib>
#include <iostream>
#include <cassert>

#include <SFML/Graphics.hpp>

#include "fancy_things.hpp"
#include "logging.h"

int main(const int argc, const char** argv) {
    LoggingStatus log_status = LoggingSetup("fancy_thing.log");
    assert(log_status == kLoggingStatus_Ok);

    if (argc < 2) {
        Log("0 files had been passed\n");
        std::cerr << "0 files had been passed" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Image image;
    bool is_load_successful = image.loadFromFile(std::string(argv[1]));
    if (!is_load_successful) {
        Log("file load wherent successful\n");
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
