#ifndef FANCY_THINGS_HPP_
#define FANCY_THINGS_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace FancyThings {
    static const char* WindowName = "fancy thigs";
    static const unsigned int WindowWidth  = 1920;
    static const unsigned int WindowHeight = 1080; 

    struct RenderState {
        bool use_algo;
    };

    void MainLoop(sf::RenderWindow* window, 
                  sf::Image* image, 
                  FancyThings::RenderState* render_state);
} // namespace FancyThings 

#endif // FANCY_THINGS_HPP_
