#include "enemy.hpp"
#include <cmath>

Enemy::Enemy(const sf::Texture& texture,
             const sf::Vector2f& position,
             const AppearDirection& appearDirection,
             const int& hp,
             const sf::Vector2u& windowSize,
             const sf::Texture& projectile,
             std::vector<std::unique_ptr<Projectile> >& projectiles,
             const int& patternChoice)
    : Ship(texture,
           position,
           appearDirection,
           windowSize,
           150,
           projectile,
           projectiles),
      hp(hp),
      markedForDeletion(false),
      isRetreating(false),
      patternChoice(patternChoice) {
  setScale(2 * getScale().x, 2 * -getScale().y);

  this->patternChoice %= 6;
  shootAmount = patternChoice >= 2 ? 1 : 3;
}

void Enemy::update(const float& deltaTime,
                   sf::RenderWindow& window,
                   const sf::Vector2f& playerPosition) {
  Ship::update(deltaTime);

  if (!isAppearing && shootAmount &&
      projectileCooldown.getElapsedTime().asSeconds() >= 0.75) {
    switch (patternChoice) {
      case 0:
      default:
        if (shootAmount == 3)
          aimAt = playerPosition;
        shootAtPoint(aimAt);
        break;
      case 1:
        aimAt = playerPosition;
        shootAtPoint(aimAt);
        break;
      case 2:
        shootCircle(CircleShootPattern::straight);
        break;
      case 3:
        shootCircle(CircleShootPattern::rotateAll);
        break;
      case 4:
        shootCircle(CircleShootPattern::rotateAllReversed);
        break;
      case 5:
        shootCircle(CircleShootPattern::rotateAlternately);
        break;
    }
    projectileCooldown.restart();
    shootAmount--;
  }

  if (!isAppearing && !shootAmount) {
  }

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
}

void Enemy::shootAtPoint(const sf::Vector2f& target) {
  float angle =
      std::atan2((target.y - getPosition().y), (target.x - getPosition().x)) +
      M_PI_2;

  angle = angle * 180 / M_PI;

  projectiles.emplace_back(std::make_unique<Projectile>(projectile, 300, angle,
                                                        false, getPosition()));
}

void Enemy::shootCircle(const CircleShootPattern& pattern) {
  float angle, rotation;

  for (int i = 0; i < 360; i += 10) {
    switch (pattern) {
      case CircleShootPattern::rotateAll:
        rotation = 0.1;
        angle = i + 10;
        break;

      case CircleShootPattern::rotateAllReversed:
        rotation = -0.1;
        angle = i + 10;
        break;

      case CircleShootPattern::rotateAlternately:
        rotation = 0.1 * std::pow((-1), i / 10);
        angle = i + 10;
        break;

      case CircleShootPattern::straight:
      default:
        rotation = 0;
        angle = i;
    }
    projectiles.emplace_back(std::make_unique<Projectile>(
        projectile, 300, angle, false, getPosition(), rotation));
  }
};
