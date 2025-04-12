#ifndef FANCY_THINGS_HPP_
#define FANCY_THINGS_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "bridge/thread_bridge.hpp"

namespace fcy {
 
void MainLoop(brg::ThreadBridge* thread_bridge);

} // namespace fcy

#endif // FANCY_THINGS_HPP_
