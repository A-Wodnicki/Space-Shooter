#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>

class Background {
 public:
  Background(sf::RenderWindow& window, const sf::Texture& texture);
  void scroll(sf::RenderWindow& window, const sf::Time& elapsed);
  void set_scroll_speed(const float& speed);

 private:
  std::vector<std::unique_ptr<sf::Sprite>> background_sprites;
  float scroll_speed = 300;
};

#endif  // BACKGROUND_H
