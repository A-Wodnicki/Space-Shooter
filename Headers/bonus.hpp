#ifndef BONUS_HPP
#define BONUS_HPP

#include <SFML/Graphics.hpp>
class Bonus : public sf::Sprite {
 public:
  Bonus(const sf::Texture& texture,
        const bool& powerUp,
        const sf::Vector2f& startingPosition);
  void update(const float& deltaTime, sf::RenderWindow& window);
  bool isMarkedForDeletion();
  void markForDeletion();
  bool isPowerUp() const;

 private:
  bool powerUp;
  sf::Vector2f velocity;
  bool markedForDeletion;
};

#endif  // BONUS_HPP
