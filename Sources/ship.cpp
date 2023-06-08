#include "ship.hpp"

Ship::Ship(const sf::Texture& texture,
           const sf::Vector2f& position,
           const AppearDirection& appearDirection,
           const sf::Vector2u& windowSize,
           const float& speed,
           const sf::Texture& projectile,
           std::vector<std::unique_ptr<Projectile> >& projectiles)
    : sf::Sprite(texture),
      targetPosition(position),
      appearDirection(appearDirection),
      isAppearing(true),
      frames(4),
      isTurning(false),
      idleAnimationFrames(2),
      turningAnimationFrames(2),
      isFlippedHorizontally(false),
      speed(speed),
      projectile(projectile),
      projectiles(projectiles) {
  for (size_t i = 0; i < idleAnimationFrames.size(); ++i) {
    idleAnimationFrames[i] =
        sf::IntRect(i * texture.getSize().x / frames, 0,
                    texture.getSize().x / frames, texture.getSize().y);
    turningAnimationFrames[i] =
        sf::IntRect((i + 2) * texture.getSize().x / frames, 0,
                    texture.getSize().x / frames, texture.getSize().y);
  }

  sf::Vector2f startingPosition;
  switch (this->appearDirection) {
    case AppearDirection::Top:
      startingPosition = {targetPosition.x, -getGlobalBounds().height};
      break;
    case AppearDirection::Right:
      startingPosition = {static_cast<float>(windowSize.x), targetPosition.y};
      break;
    case AppearDirection::Bottom:
      startingPosition = {targetPosition.x, static_cast<float>(windowSize.y)};
      break;
    case AppearDirection::Left:
      startingPosition = {-getGlobalBounds().width, targetPosition.y};
      break;
  }
  setPosition(startingPosition);

  if (this->appearDirection == AppearDirection::Left ||
      this->appearDirection == AppearDirection::Right)
    currentFrame = turningAnimationFrames.begin();
  else
    currentFrame = idleAnimationFrames.begin();

  setTextureRect(*currentFrame);
  setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
}

void Ship::update(const float& deltaTime) {
  if (isAppearing)
    appear(deltaTime);

  animate();
}

void Ship::animate() {
  if (animationTimer.getElapsedTime().asSeconds() <= 0.05)
    return;
  animationTimer.restart();
  currentFrame++;
  if (currentFrame == idleAnimationFrames.end())
    currentFrame = idleAnimationFrames.begin();
  if (currentFrame == turningAnimationFrames.end())
    currentFrame = turningAnimationFrames.begin();

  setTextureRect(*currentFrame);
}

void Ship::appear(const float& deltaTime) {
  switch (appearDirection) {
    case AppearDirection::Top:
      if (targetPosition.y <= getPosition().y)
        isAppearing = false;
      moveDown(deltaTime);
      break;
    case AppearDirection::Right:
      if (targetPosition.x >= getPosition().x) {
        isAppearing = false;
        currentFrame = idleAnimationFrames.begin();
      }
      moveLeft(deltaTime);
      break;
    case AppearDirection::Bottom:
      if (targetPosition.y >= getPosition().y)
        isAppearing = false;
      moveUp(deltaTime);
      break;
    case AppearDirection::Left:
      if (targetPosition.x <= getPosition().x) {
        isAppearing = false;
        currentFrame = idleAnimationFrames.begin();
      }
      moveRight(deltaTime);
      break;
  }
}

void Ship::flipSpriteHorizontally() {
  setScale(-getScale().x, getScale().y);
  isFlippedHorizontally = !isFlippedHorizontally;
}

void Ship::changeAnimationType() {
  if (isTurning) {
    currentFrame = turningAnimationFrames.begin();
    return;
  }
  currentFrame = idleAnimationFrames.begin();
}

void Ship::moveUp(const float& deltaTime) {
  move(sf::Vector2f(0, -speed) * deltaTime);
}

void Ship::moveRight(const float& deltaTime) {
  if (!isFlippedHorizontally)
    flipSpriteHorizontally();
  move(sf::Vector2f(speed, 0) * deltaTime);
}

void Ship::moveDown(const float& deltaTime) {
  move(sf::Vector2f(0, speed) * deltaTime);
}

void Ship::moveLeft(const float& deltaTime) {
  if (isFlippedHorizontally)
    flipSpriteHorizontally();
  move(sf::Vector2f(-speed, 0) * deltaTime);
}
