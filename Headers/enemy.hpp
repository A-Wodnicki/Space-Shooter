#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "ship.hpp"

enum class CircleShootPattern {
  straight,
  rotateAll,
  rotateAllReversed,
  rotateAlternately
};

class Enemy : public Ship {
 public:
  Enemy(const sf::Texture& texture,
        const sf::Vector2f& position,
        const AppearDirection& appearDirection,
        const int& hp,
        const sf::Vector2u& windowSize,
        const sf::Texture& projectile,
        std::vector<std::unique_ptr<Projectile>>& projectiles,
        const int& patternChoice);

  void update(const float& deltaTime,
              sf::RenderWindow& window,
              const sf::Vector2f& playerPosition);

  int getHp() const;
  void setHp(const int& hp);

  void markForDeletion();
  bool isMarkedForDeletion() const;

 private:
  void shoot(const sf::Vector2f& playerPosition);
  void shootAtPoint(const sf::Vector2f& target, const float& projectileSpeed);
  void shootCircle(const CircleShootPattern& pattern);

  void retreat(const float& deltaTime, const sf::Vector2u& windowSize);

  int hp;
  bool markedForDeletion;

  sf::Clock shootCountdown;
  sf::Clock projectileCooldown;

  int patternChoice;
  int shootAmount;
  sf::Vector2f aimAt;

  sf::Clock retreatCountdown;
  bool retreating;
};

#endif  // ENEMY_HPP
