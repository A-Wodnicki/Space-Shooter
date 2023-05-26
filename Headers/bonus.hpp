#ifndef BONUS_HPP
#define BONUS_HPP

#include <SFML/Graphics.hpp>
class Bonus : public sf::Sprite {
 public:
  Bonus(const sf::Texture& texture,
        const bool& isPowerUp,
        const sf::Vector2f& startingPosition);
  void update(const float& deltaTime);
  bool isMarkedForDeletion();
  void markForDeletion();

 private:
  bool isPowerUp;
  sf::Vector2f velocity;
  bool markedForDeletion;
};

#endif  // BONUS_HPP
