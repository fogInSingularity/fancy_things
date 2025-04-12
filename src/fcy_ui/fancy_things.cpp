#include "fcy_ui/fancy_things.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <spdlog/spdlog.h>

#include "helpers/trace_calls.hpp"
#include "bridge/thread_bridge.hpp"
#include "filter/pixel.hpp"
#include "filter/utility.hpp"
#include "filter/matrix.hpp"
#include "fcy_ui/fcy_assert.hpp"
#include "fcy_ui/ui_config.hpp"

namespace fcy {

// static ----------------------------------------------------------------------

static void CheckEvents(sf::RenderWindow* window);
static sf::Sprite ConvertMatToSprite(const ftr::Matrix<ftr::PixelU>& mat, sf::Texture* image_texture);

static std::string GenerateUniqueFileName(const std::string& file_prefix, const std::string& file_extension);

// global ----------------------------------------------------------------------

void MainLoop(brg::ThreadBridge* thread_bridge) {
    fcy_assert(thread_bridge != nullptr);

    hlp::trace_call();

    ftr::Size image_size = thread_bridge->ImageSize();

    auto small_image_size = ftr::SizeT<unsigned>{image_size};
    sf::RenderWindow window(sf::VideoMode(small_image_size.w,
                                          small_image_size.h), 
                            fcy::WindowName);

    sf::Texture image_texture{};
    sf::Sprite image_sprite{};

    auto raw_image = thread_bridge->CurrentImage();
    image_sprite = ConvertMatToSprite(raw_image, &image_texture);

    while (window.isOpen()) {
        if (thread_bridge->IsUpdated()) {
            spdlog::debug("Main loop: image is updated");
            raw_image = thread_bridge->CurrentImage();
            image_sprite = ConvertMatToSprite(raw_image, &image_texture);
        }

        window.clear();
        window.draw(image_sprite);
        window.display();

        CheckEvents(&window);
    }

    thread_bridge->IsFinished(true);
}

// static ----------------------------------------------------------------------

static void CheckEvents(sf::RenderWindow* window) {
    fcy_assert(window != nullptr);

    sf::Event event{};
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            spdlog::debug("button close pressed");
            window->close();
        } else if (event.type == sf::Event::KeyPressed) {
            spdlog::debug("key pressed");
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                spdlog::trace("pressed Q");
                window->close();      
            }
        }
    }
}

static sf::Sprite ConvertMatToSprite(const ftr::Matrix<ftr::PixelU>& mat, sf::Texture* image_texture) {
    hlp::trace_call();

    ftr::Size image_size = mat.GetSize();


    auto small_image_size = ftr::SizeT<unsigned>{image_size};

    sf::Image image{};
    image.create(small_image_size.w, small_image_size.h, reinterpret_cast<const sf::Uint8*>(mat.GetData()));

    image_texture->loadFromImage(image);
    sf::Sprite image_sprite{};
    image_sprite.setTexture(*image_texture);

    return image_sprite;
}

static std::string GenerateUniqueFileName(const std::string& file_prefix, const std::string& file_extension) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream unique_name_time;
    unique_name_time << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S"); // Format time

    return file_prefix + "_" + unique_name_time.str() + "." + file_extension;
}

} // namespace fcy
