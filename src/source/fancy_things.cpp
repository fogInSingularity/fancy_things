#include "fancy_things.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "spdlog/spdlog.h"

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
        BoxBlurFilter filter;
        raw_image.Filter(&filter);
        render_state->use_algo = false;
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
            window->close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                window->close();      
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                render_state->use_algo = true;
            }
        }
    }
}

} // namespace fcy
