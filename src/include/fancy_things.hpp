#ifndef FANCY_THINGS_HPP_
#define FANCY_THINGS_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace fcy {

inline const char* WindowName = "fancy thigs";
inline const unsigned int WindowWidth  = 1920;
inline const unsigned int WindowHeight = 1080; 

struct RenderState {
    bool use_algo;
    bool save_image;
};

void MainLoop(sf::RenderWindow* window, 
              sf::Image* image, 
              fcy::RenderState* render_state);

} // namespace fcy

#endif // FANCY_THINGS_HPP_
