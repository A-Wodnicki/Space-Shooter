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
      patternChoice(patternChoice),
      retreating(false) {
  setScale(2 * getScale().x, 2 * -getScale().y);

  this->patternChoice %= 6;
  shootAmount = patternChoice >= 2 ? 1 : 3;
}

void Enemy::update(const float& deltaTime,
                   sf::RenderWindow& window,
                   const sf::Vector2f& playerPosition) {
  Ship::update(deltaTime);

  if (appearing)
    shootCountdown.restart();

  if (shootCountdown.getElapsedTime().asSeconds() >= 3 && shootAmount &&
      projectileCooldown.getElapsedTime().asSeconds() >= 1.f / 3) {
    switch (patternChoice) {
      case 0:
      default:
        if (shootAmount == 3)
          aimAt = playerPosition;
        shootAtPoint(aimAt, 400);
        break;
      case 1:
        aimAt = playerPosition;
        shootAtPoint(aimAt, 300);
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

  if (shootAmount)
    retreatCountdown.restart();

  if (retreatCountdown.getElapsedTime().asSeconds() >= 3)
    retreat(2 * deltaTime, window.getSize());

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

void Enemy::shootAtPoint(const sf::Vector2f& target,
                         const float& projectileSpeed) {
  float angle =
      std::atan2((target.y - getPosition().y), (target.x - getPosition().x)) +
      M_PI_2;

  angle = angle * 180 / M_PI;

  projectiles.emplace_back(std::make_unique<Projectile>(
      projectile, projectileSpeed, angle, false, getPosition()));
}

void Enemy::shootCircle(const CircleShootPattern& pattern) {
  float angle, rotation, speed;

  for (int i = 0; i < 360; i += 10) {
    switch (pattern) {
      case CircleShootPattern::rotateAll:
        speed = 200;
        rotation = 1;
        angle = i + 10;
        break;

      case CircleShootPattern::rotateAllReversed:
        speed = 200;
        rotation = -1;
        angle = i + 10;
        break;

      case CircleShootPattern::rotateAlternately:
        speed = 150;
        rotation = 1 * std::pow((-1), i / 10);
        angle = i + 10;
        break;

      case CircleShootPattern::straight:
      default:
        speed = 250;
        rotation = 0;
        angle = i;
    }
    projectiles.emplace_back(std::make_unique<Projectile>(
        projectile, speed, angle, false, getPosition(), rotation));
  }
}

void Enemy::retreat(const float& deltaTime, const sf::Vector2u& windowSize) {
  float positionOffset = getPosition().y <= windowSize.y / 4
                             ? getGlobalBounds().height
                             : getGlobalBounds().width;

  sf::Vector2f position = getPosition();

  if (getPosition().y <= windowSize.y / 4) {
    moveUp(deltaTime);
    position.y += positionOffset;
  } else if (getPosition().x <= windowSize.x / 2) {
    moveLeft(deltaTime);
    position.x += positionOffset;
    if (!retreating) {
      turning = true;
      changeAnimationType();
    }
  } else {
    moveRight(deltaTime);
    position.x -= positionOffset;
    if (!retreating) {
      turning = true;
      changeAnimationType();
    }
  }

  retreating = true;

  if (!sf::FloatRect(0, 0, windowSize.x, windowSize.y).contains(position)) {
    markForDeletion();
  }
};
