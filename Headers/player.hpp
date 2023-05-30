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

 private:
  void handleControl(const float& deltaTime);
  void shoot(int angle);
  void shootNarrow();
  void shootWide();

  sf::Keyboard::Key shootButton1 = sf::Keyboard::Key::J;
  sf::Keyboard::Key shootButton2 = sf::Keyboard::Key::K;
  const sf::Texture& projectile;
  sf::Clock projectileCooldown;
  int hp;
};

#endif  // PLAYER_HPP
