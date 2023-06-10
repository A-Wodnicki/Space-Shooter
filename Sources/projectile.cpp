#include "projectile.hpp"

Projectile::Projectile(const sf::Texture& texture,
                       const float& speed,
                       const float& angle,
                       const bool& isPlayer,
                       const sf::Vector2f& startingPosition,
                       const float& rotation)
    : sf::Sprite(texture),
      rotation(rotation),
      playerProjectile(isPlayer),
      markedForDeletion(false),
      bright(false) {
  setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
  setRotation(angle);
  setPosition(startingPosition);

  sf::Transform rotationTransform;
  rotationTransform.rotate(angle);
  velocity = rotationTransform.transformPoint(sf::Vector2f(0, -1)) * speed;
}

void Projectile::update(const float& deltaTime, sf::RenderWindow& window) {
  this->rotate(rotation);
  move(velocity * deltaTime);
  flash();
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
  if (!angle)
    return;

  if (rotationTimer.getElapsedTime().asSeconds() < 0.1)
    return;

  rotationTimer.restart();

  sf::Transform transform;
  transform.rotate(angle);
  velocity = transform.transformPoint(velocity);
  sf::Sprite::rotate(angle);
}

void Projectile::flash() {
  if (flashTimer.getElapsedTime().asSeconds() < 0.1)
    return;

  flashTimer.restart();

  sf::Color color = bright ? sf::Color::White : sf::Color(255, 255, 255, 200);

  setColor(color);

  bright = !bright;
}
