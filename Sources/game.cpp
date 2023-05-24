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

  texture = std::make_unique<sf::Texture>();
  if (!texture->loadFromFile("assets/images/ship.png"))
    window.close();
  textures["ship"] = std::move(texture);

  ship = std::make_unique<sf::Sprite>();
  ship->setTexture(*textures["ship"]);
  animation_frames.emplace_back(
      0, 0, static_cast<int>(ship->getLocalBounds().width) / 4,
      static_cast<int>(ship->getLocalBounds().height));
  animation_frames.emplace_back(
      static_cast<int>(ship->getLocalBounds().width) / 4, 0,
      static_cast<int>(ship->getLocalBounds().width) / 4,
      static_cast<int>(ship->getLocalBounds().height));
  animation_frames.emplace_back(
      static_cast<int>(ship->getLocalBounds().width) / 4 * 2, 0,
      static_cast<int>(ship->getLocalBounds().width) / 4,
      static_cast<int>(ship->getLocalBounds().height));
  animation_frames.emplace_back(
      static_cast<int>(ship->getLocalBounds().width) / 4 * 3, 0,
      static_cast<int>(ship->getLocalBounds().width) / 4,
      static_cast<int>(ship->getLocalBounds().height));
  ship->setTextureRect({0, 0,
                        static_cast<int>(ship->getLocalBounds().width) / 4,
                        static_cast<int>(ship->getLocalBounds().height)});
  ship->setPosition(window.getSize().x / 2, window.getSize().y / 4 * 3);
  ship->setOrigin(ship->getGlobalBounds().width / 2, 0);
}

void Game::loop() {
  sf::Clock clock;

  float animation_countdown = 0;
  int ship_speed_x = 300;
  int ship_speed_y = 300;

  while (window.isOpen()) {
    sf::Time elapsed = clock.restart();
    animation_countdown += elapsed.asSeconds();

    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();

    background->scroll(window, elapsed);

    while (animation_countdown >= 0.05) {
      if (ship_animation) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ==
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
          ship->setTextureRect(animation_frames[0]);
        else
          ship->setTextureRect(animation_frames[2]);
      } else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ==
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
          ship->setTextureRect(animation_frames[1]);
        else
          ship->setTextureRect(animation_frames[3]);
      }
      ship_animation = !ship_animation;
      animation_countdown -= 0.05;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
      {
        ship->setScale(-abs(ship->getScale().x), 1);
        ship_speed_x = abs(ship_speed_x);
      }
    } else {
      ship->setScale(abs(ship->getScale().x), 1);
      ship_speed_x = -abs(ship_speed_x);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
      ship_speed_y = abs(ship_speed_y);
    } else {
      ship_speed_y = -abs(ship_speed_y);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) !=
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
      ship->move(ship_speed_x * elapsed.asSeconds(), 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) !=
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
      ship->move(0, ship_speed_y * elapsed.asSeconds());
    }

    window.draw(*ship);

    window.display();
  }
}
