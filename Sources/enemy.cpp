#include "enemy.hpp"

Enemy::Enemy(const sf::Texture& texture,
             const sf::Vector2f& position,
             const AppearDirection& appeardDirection,
             const int& hp,
             const sf::Vector2u& windowSize,
             const sf::Texture& projectile,
             std::vector<std::unique_ptr<Projectile> >& projectiles)
    : Ship(texture,
           position,
           appeardDirection,
           windowSize,
           150,
           projectile,
           projectiles),
      hp(hp),
      markedForDeletion(false) {
  setScale(2 * getScale().x, 2 * -getScale().y);
}

void Enemy::update(const float& deltaTime, sf::RenderWindow& window) {
  Ship::update(deltaTime);
  window.draw(*this);
}

int Enemy::getHp() const {
  return hp;
}

void Enemy::setHp(const int& hp) {
  this->hp = hp;
}

void Enemy::markForDeletion() {
  markedForDeletion = true;
}

bool Enemy::isMarkedForDeletion() const {
  return markedForDeletion;
};
