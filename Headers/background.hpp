#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>

class Background {
 public:
  Background(const sf::Vector2u& windowSize, sf::Texture& texture);

  void update(const float& deltaTime, sf::RenderWindow& window);
  void setScrollSpeed(const float& speed);

 private:
  void scroll(const float& deltaTime);
  void draw(sf::RenderWindow& window);

  std::vector<std::unique_ptr<sf::Sprite>> backgroundSprites;
  sf::Vector2u windowSize;
  float scrollSpeed;
};

#endif  // BACKGROUND_HPP
