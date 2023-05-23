#include "game.h"
#include <iostream>

Game::Game() {
  ::ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);
  window.setFramerateLimit(144);

  auto texture = std::make_unique<sf::Texture>();
  if (!texture->loadFromFile("assets/images/blue.png"))
    window.close();
  texture->setRepeated(true);

  textures["background"] = std::move(texture);

  background = std::make_unique<Background>(window, *textures["background"]);
}

void Game::loop() {
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
