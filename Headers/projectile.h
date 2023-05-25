#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <cmath>

class Projectile : public sf::Sprite {
 private:
  sf::Vector2f velocity;
  int damage;
  bool playerProjectile;
  bool markedForDeletion;
  sf::Clock flashTimer;
  void flash();
  bool bright;

 public:
  Projectile(const sf::Texture& texture,
             const float& speed,
             float angle,
             int damage,
             bool isPlayer,
             const sf::Vector2f& startingPosition);
  void update(float deltaTime);
  int getDamage() const;
  bool isPlayerOwned() const;
  bool isMarkedMarkedForDeletion() const;
  void markForDeletion();
};
#endif  // PROJECTILE_H
