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
  void setHp(const int& hp);

  float getHurtCooldown() const;
  void restartHurtCooldown();

  int getScoreMultiplier() const;

  int getPowerUpCount() const;
  void addPowerUp();

  bool isSuperOnCooldown() const;

 private:
  void handleControl(const float& deltaTime, const sf::Vector2u& windowSize);
  void shoot(int angle, int projectilesToShoot = 4);
  void shootSuper();

  sf::Keyboard::Key moveUpButton = sf::Keyboard::Key::W,
                    moveLeftButton = sf::Keyboard::Key::A,
                    moveDownButton = sf::Keyboard::Key::S,
                    moveRightButton = sf::Keyboard::Key::D,
                    shootButton1 = sf::Keyboard::Key::J,
                    shootButton2 = sf::Keyboard::Key::K,
                    shootButton3 = sf::Keyboard::Key::L;

  sf::Clock projectileCooldown;
  int projectileAngle;

  int hp;
  sf::Clock hurtCooldown;

  int scoreMultiplier;

  int powerUpCount;
  sf::Clock superDuration;
  sf::Clock superCooldown;
  bool superOnCooldown;
  bool shootingSuper;
};

#endif  // PLAYER_HPP
