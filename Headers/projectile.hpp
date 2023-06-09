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
  int getDamage() const;
  bool isPlayerOwned() const;
  bool isMarkedMarkedForDeletion() const;
  void markForDeletion();
  void rotate(const float& angle);

 private:
  void flash();

  sf::Vector2f velocity;
  bool playerProjectile;
  bool markedForDeletion;
  sf::Clock flashTimer;
  bool bright;
  float rotation;
  sf::Clock rotationTimer;
};
#endif  // PROJECTILE_HPP
