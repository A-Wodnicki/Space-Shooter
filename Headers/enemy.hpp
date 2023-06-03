#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "ship.hpp"

class Enemy : public Ship {
 public:
  Enemy(const sf::Texture& texture,
        const sf::Vector2f& position,
        const AppearDirection& appeardDirection,
        const int& hp,
        const sf::Vector2u& windowSize,
        const sf::Texture& projectile,
        std::vector<std::unique_ptr<Projectile>>& projectiles);

  void update(const float& deltaTime, sf::RenderWindow& window);

  int getHp() const;
  void setHp(const int& hp);

  void markForDeletion();
  bool isMarkedForDeletion() const;

 private:
  int hp;
  bool markedForDeletion;
};

#endif  // ENEMY_HPP
