#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <SFML/Graphics.hpp>

class Projectile : public sf::Sprite {
 public:
  Projectile(const sf::Texture& texture,
             const float& speed,
             const float& angle,
             const bool& isPlayer,
             const sf::Vector2f& startingPosition,
             const float& rotation = 0);

  void update(const float& deltaTime, sf::RenderWindow& window);

  bool isPlayerOwned() const;

  bool isMarkedMarkedForDeletion() const;
  void markForDeletion();

 private:
  void rotate(const float& angle);

  void flash();

  sf::Vector2f velocity;

  float rotation;
  sf::Clock rotationTimer;

  bool playerProjectile;
  bool markedForDeletion;

  bool bright;
  sf::Clock flashTimer;
};
#endif  // PROJECTILE_HPP
