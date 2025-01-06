#include "fancy_things.hpp"

#include <cassert>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "filters.hpp"
#include "to_str.hpp"

// static ----------------------------------------------------------------------

static const size_t kRGBAPixelSize = 4;

static void RenderLoop(sf::RenderWindow* window, 
                       FancyThings::RenderState* render_state, 
                       const sf::Sprite& image_sprite);
static void CheckEvents(sf::RenderWindow* window, 
                        FancyThings::RenderState* render_state);

// global ----------------------------------------------------------------------

void FancyThings::MainLoop(sf::RenderWindow* window, 
                           sf::Image* image, 
                           FancyThings::RenderState* render_state) {
    assert(window != nullptr);
    assert(image != nullptr);
    assert(render_state != nullptr);
                            
    sf::Texture image_texture;
    image_texture.loadFromImage(*image);

    sf::Sprite image_sprite;
    image_sprite.setTexture(image_texture);         

    RenderLoop(window, render_state, image_sprite);

    sf::Vector2u image_size = image->getSize();
    spdlog::debug("%s: %u, %s: %u\n", TO_STR(image_size.x), image_size.x, TO_STR(image_size.y), image_size.y);

    const size_t raw_image_size = static_cast<size_t>(image_size.x * image_size.y) * kRGBAPixelSize;
    sf::Uint8* raw_image = new sf::Uint8[raw_image_size];
    std::copy(image->getPixelsPtr(), 
              image->getPixelsPtr() + raw_image_size, 
              raw_image);
    
    if (render_state->use_algo) {
        // Filters::GaussianBlur(reinterpret_cast<Pixel*>(raw_image), image_size.x, image_size.y);
        // Filters::ThresholdFilter(raw_image, raw_image_size);
        Filters::ReversFilter(reinterpret_cast<Pixel*>(raw_image), image_size.x, image_size.y);
        render_state->use_algo = false;
    }

    image->create(image_size.x, image_size.y, raw_image);
    delete [] raw_image;

    window->setSize(image_size);
}

// static ----------------------------------------------------------------------

static void RenderLoop(sf::RenderWindow* window, 
                       FancyThings::RenderState* render_state, 
                       const sf::Sprite& image_sprite) {
    assert(window != nullptr);
    assert(render_state != nullptr);
    
    while (window->isOpen() && !render_state->use_algo) {
        CheckEvents(window, render_state);

        // render
        window->clear();
        window->draw(image_sprite);
        window->display();
    }
}

static void CheckEvents(sf::RenderWindow* window, 
                        FancyThings::RenderState* render_state) {
    assert(window != nullptr);
    assert(render_state != nullptr);

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

