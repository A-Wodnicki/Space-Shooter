#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ship.hpp"

class Player : public Ship {
 public:
  Player(const sf::Texture& texture,
         const sf::Texture& projectile,
         const sf::Vector2u& windowSize,
         std::vector<std::unique_ptr<Projectile>>& projectiles);

  void update(const float& deltaTime, sf::RenderWindow& window);
  int getHp() const;
  int getPowerUpCount() const;
  bool isSuperOnCooldown() const;

  void setHp(const int& hp);
  void addPowerUp();

 private:
  void handleControl(const float& deltaTime);
  void shoot(int angle, int projectilesToShoot = 4);
  void shootSuper();

  sf::Keyboard::Key moveUpButton = sf::Keyboard::Key::W,
                    moveLeftButton = sf::Keyboard::Key::A,
                    moveDownButton = sf::Keyboard::Key::S,
                    moveRightButton = sf::Keyboard::Key::D,
                    shootButton1 = sf::Keyboard::Key::J,
                    shootButton2 = sf::Keyboard::Key::K,
                    shootButton3 = sf::Keyboard::Key::L;
  const sf::Texture& projectile;
  sf::Clock projectileCooldown;
  int projectileAngle;
  int hp;
  int powerUpCount;
  sf::Clock superDuration;
  sf::Clock superCooldown;
  bool superOnCooldown;
  bool shootingSuper;
};

#endif  // PLAYER_HPP
