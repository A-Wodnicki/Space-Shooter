#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "background.h"

class Game {
 public:
  Game();
  void loop();

 private:
  sf::RenderWindow window{sf::VideoMode::getDesktopMode(), "Space Shooter"};
  std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
  std::unique_ptr<Background> background;
  std::unique_ptr<sf::Sprite> ship;
  bool ship_animation = false;
  std::vector<sf::IntRect> animation_frames;
};

#endif  // GAMEENGINE_H
