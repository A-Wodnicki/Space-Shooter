#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ship.hpp"

class Player : public Ship {
 public:
  Player(const sf::Texture& texture,
         const sf::Texture& projectile,
         const sf::Vector2u& windowSize,
         std::vector<std::unique_ptr<Projectile>>& projectiles);

  void update(const float& deltaTime);
  int getHp() const;
  int getPowerUpCount() const;

  void addHp();
  void addPowerUp();

 private:
  void handleControl(const float& deltaTime);
  void shoot(int angle, int projectilesToShoot = 4);
  void shootSuper();

  sf::Keyboard::Key shootButton1 = sf::Keyboard::Key::J;
  sf::Keyboard::Key shootButton2 = sf::Keyboard::Key::K;
  const sf::Texture& projectile;
  sf::Clock projectileCooldown;
  int projectileAngle = 10;
  int hp;
  int powerUpCount;
  sf::Clock superDuration;
  sf::Clock superCooldown;
  bool shootingSuper;
};

#endif  // PLAYER_HPP
