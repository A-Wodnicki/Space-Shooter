#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <SFML/Graphics.hpp>

class Projectile : public sf::Sprite {
 public:
  Projectile(const sf::Texture& texture,
             const float& speed,
             const float& angle,
             const int& damage,
             const bool& isPlayer,
             const sf::Vector2f& startingPosition);

  void update(const float& deltaTime);
  int getDamage() const;
  bool isPlayerOwned() const;
  bool isMarkedMarkedForDeletion() const;
  void markForDeletion();
  void rotate(const float& angle);

 private:
  void flash();

  sf::Vector2f velocity;
  int damage;
  bool playerProjectile;
  bool markedForDeletion;
  float flashTimer;
  bool bright;
};
#endif  // PROJECTILE_HPP
