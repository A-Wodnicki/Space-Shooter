#include "projectile.hpp"

Projectile::Projectile(const sf::Texture& texture,
                       const float& speed,
                       const float& angle,
                       const bool& isPlayer,
                       const sf::Vector2f& startingPosition,
                       const float& rotation)
    : sf::Sprite(texture),
      playerProjectile(isPlayer),
      markedForDeletion(false),
      bright(false),
      rotation(rotation) {
  setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
  setRotation(angle);
  setPosition(startingPosition);

  sf::Transform rotationTransform;
  rotationTransform.rotate(angle);
  velocity = rotationTransform.transformPoint(sf::Vector2f(0, -1)) * speed;
}

void Projectile::update(const float& deltaTime, sf::RenderWindow& window) {
  move(velocity * deltaTime);

  if (flashTimer.getElapsedTime().asSeconds() >= 0.01) {
    flash();
    flashTimer.restart();
  }

  this->rotate(rotation);

  window.draw(*this);
}

bool Projectile::isPlayerOwned() const {
  return playerProjectile;
}

bool Projectile::isMarkedMarkedForDeletion() const {
  return markedForDeletion;
}

void Projectile::markForDeletion() {
  markedForDeletion = true;
}

void Projectile::rotate(const float& angle) {
  sf::Transform transform;
  transform.rotate(angle);
  velocity = transform.transformPoint(velocity);
  sf::Sprite::rotate(angle);
}

void Projectile::flash() {
  if (bright)
    setColor(sf::Color(255, 255, 255, 255));
  else
    setColor(sf::Color(255, 255, 255, 200));
  bright = !bright;
}
