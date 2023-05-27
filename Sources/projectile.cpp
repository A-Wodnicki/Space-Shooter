#include "projectile.hpp"

Projectile::Projectile(const sf::Texture& texture,
                       const float& speed,
                       const float& angle,
                       const int& damage,
                       const bool& isPlayer,
                       const sf::Vector2f& startingPosition)
    : sf::Sprite(texture),
      damage(damage),
      playerProjectile(isPlayer),
      markedForDeletion(false),
      flashTimer(0),
      bright(false) {
  setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
  setRotation(angle);
  setPosition(startingPosition);

  sf::Transform rotationTransform;
  rotationTransform.rotate(angle);
  velocity = rotationTransform.transformPoint(sf::Vector2f(0, -1)) * speed;
}

void Projectile::update(const float& deltaTime) {
  move(velocity * deltaTime);
  flashTimer += deltaTime;

  if (flashTimer >= 0.01) {
    flash();
    flashTimer = 0;
  }
}

int Projectile::getDamage() const {
  return damage;
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
