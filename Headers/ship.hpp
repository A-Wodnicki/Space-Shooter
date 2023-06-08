#ifndef SHIP_HPP
#define SHIP_HPP

#include <SFML/Graphics.hpp>
#include "projectile.hpp"

enum class AppearDirection { Top, Right, Bottom, Left };

class Ship : public sf::Sprite {
 public:
  Ship(const sf::Texture& texture,
       const sf::Vector2f& position,
       const AppearDirection& appearDirection,
       const sf::Vector2u& windowSize,
       const float& speed,
       const sf::Texture& projectile,
       std::vector<std::unique_ptr<Projectile>>& projectiles);

  virtual ~Ship() = default;

  void update(const float& deltaTime);

 protected:
  void animate();
  void appear(const float& deltaTime);
  void flipSpriteHorizontally();
  void changeAnimationType();

  void moveUp(const float& deltaTime);
  void moveRight(const float& deltaTime);
  void moveDown(const float& deltaTime);
  void moveLeft(const float& deltaTime);

  sf::Vector2f targetPosition;
  AppearDirection appearDirection;
  bool isAppearing;

  int frames;
  bool isTurning;
  std::vector<sf::IntRect> idleAnimationFrames;
  std::vector<sf::IntRect> turningAnimationFrames;
  std::vector<sf::IntRect>::iterator currentFrame;
  sf::Clock animationTimer;
  bool isFlippedHorizontally;

  float speed;

  const sf::Texture& projectile;
  std::vector<std::unique_ptr<Projectile>>& projectiles;
};

#endif  // SHIP_HPP
