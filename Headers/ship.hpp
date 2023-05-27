#ifndef SHIP_HPP
#define SHIP_HPP

#include <SFML/Graphics.hpp>

enum class AppearDirection { Top, Right, Bottom, Left };

class Ship : public sf::Sprite {
 public:
  Ship(const sf::Texture& texture,
       const sf::Vector2f& position,
       const AppearDirection& appearDirection,
       const sf::Vector2u& windowSize,
       const float& speed);

  virtual ~Ship() = default;

  void update(const float& deltaTime);

  void moveUp(const float& deltaTime);
  void moveRight(const float& deltaTime);
  void moveDown(const float& deltaTime);
  void moveLeft(const float& deltaTime);

 protected:
  void animate();
  void appear(const float& deltaTime);
  void flipSpriteHorizontally();

  sf::Vector2f targetPositon;
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
};

#endif  // SHIP_HPP
