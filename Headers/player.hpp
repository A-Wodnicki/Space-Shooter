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

 private:
  void handleControl(const float& deltaTime);
  sf::Keyboard::Key shootButton = sf::Keyboard::Key::Space;
  const sf::Texture& projectile;
  sf::Clock projectileCooldown;
};

#endif  // PLAYER_HPP
