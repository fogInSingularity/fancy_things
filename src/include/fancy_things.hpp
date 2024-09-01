#ifndef FANCY_THINGS_HPP_
#define FANCY_THINGS_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "attr.h"

namespace FancyThings {
    UNUSED static const char* WindowName = "fancy thigs";
    UNUSED static const unsigned int WindowWidth  = 1920;
    UNUSED static const unsigned int WindowHeight = 1080; 

    struct RenderState {
        bool use_algo;
    };

    void MainLoop(sf::RenderWindow* window, 
                  sf::Image* image, 
                  FancyThings::RenderState* render_state);
} // namespace FancyThings 

#endif // FANCY_THINGS_HPP_
