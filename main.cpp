#include <windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "background.h"

int main() {
  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Space Shooter");
  ::ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);
  window.setFramerateLimit(144);

  auto bg_texture = std::make_unique<sf::Texture>();
  bg_texture->loadFromFile("assets/images/blue.png");
  bg_texture->setRepeated(true);

  auto background = std::make_unique<Background>(window, bg_texture);

  sf::Clock clock;

  while (window.isOpen()) {
    sf::Time elapsed = clock.restart();
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();

    background->scroll(window, elapsed);

    window.display();
  }
}
