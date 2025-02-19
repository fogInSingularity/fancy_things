#include "fancy_things.hpp"

#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <spdlog/spdlog.h>

#include "filters.hpp"
#include "to_str.hpp"
#include "fcy_assert.hpp"
#include "raw_image.hpp"

namespace fcy {

// static ----------------------------------------------------------------------

static void RenderLoop(sf::RenderWindow* window, 
                       RenderState* render_state, 
                       const sf::Sprite& image_sprite);
static void CheckEvents(sf::RenderWindow* window, 
                        RenderState* render_state);

static std::string GenerateUniqueFileName(const std::string& file_prefix, const std::string& file_extension);

// global ----------------------------------------------------------------------

void MainLoop(sf::RenderWindow* window, 
              sf::Image* image, 
              RenderState* render_state) {
    fcy_assert(window != nullptr);
    fcy_assert(image != nullptr);
    fcy_assert(render_state != nullptr);
                            
    sf::Texture image_texture;
    image_texture.loadFromImage(*image);

    sf::Sprite image_sprite;
    image_sprite.setTexture(image_texture);         

    RenderLoop(window, render_state, image_sprite);

    sf::Vector2u image_size = image->getSize();
    spdlog::debug("main render loop: {}: {}, {}: {}", TO_STR(image_size.x), image_size.x, TO_STR(image_size.y), image_size.y);
 
    RawImage raw_image(image->getPixelsPtr(), image_size.x, image_size.y);

    if (render_state->use_algo) {
        // ThresholdFilter filter;
        // ReverseFilter filter;
        // BoxBlurFilter filter;
        // GaussianBlurFilter filter;
        // MotionBlurFilter filter;
        // EmbossingFilter filter;
        EdgeDetectorSobelFilter filter;
        // EdgeDetectorLaplacianFilter filter;
        raw_image.Filter(&filter);


        render_state->use_algo = false;
    }

    if (render_state->save_image) {
        sf::Image image_to_save;
        image_to_save.create(raw_image.GetWidth(), raw_image.GetHeight(), raw_image.GetPixelPtr());

        std::string unique_file_name = GenerateUniqueFileName("images/image", "png");
        image_to_save.saveToFile(unique_file_name);

        render_state->save_image = false;
    }

    image->create(image_size.x, image_size.y, raw_image.GetPixelPtr());

    window->setSize(image_size);
}

// static ----------------------------------------------------------------------

static void RenderLoop(sf::RenderWindow* window, 
                       RenderState* render_state, 
                       const sf::Sprite& image_sprite) {
    fcy_assert(window != nullptr);
    fcy_assert(render_state != nullptr);
    
    while (window->isOpen() && !render_state->use_algo) {
        CheckEvents(window, render_state);

        window->clear();
        window->draw(image_sprite);
        window->display();
    }
}

static void CheckEvents(sf::RenderWindow* window, 
                        RenderState* render_state) {
    fcy_assert(window != nullptr);
    fcy_assert(render_state != nullptr);

    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            spdlog::debug("button close pressed");
            window->close();
        } else if (event.type == sf::Event::KeyPressed) {
            spdlog::debug("key pressed");
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                spdlog::trace("pressed Q");
                window->close();      
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                spdlog::trace("pressed A");
                render_state->use_algo = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                spdlog::trace("pressed S");
                render_state->save_image = true;
            }
        }
    }
}

static std::string GenerateUniqueFileName(const std::string& file_prefix, const std::string& file_extension) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream unique_name_time;
    unique_name_time << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S"); // Format time

    return file_prefix + "_" + unique_name_time.str() + "." + file_extension;
}

} // namespace fcy
