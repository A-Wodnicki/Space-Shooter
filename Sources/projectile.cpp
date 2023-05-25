#include "projectile.h"

Projectile::Projectile(const sf::Texture& texture,
                       const float& speed,
                       float angle,
                       int damage,
                       bool isPlayer,
                       const sf::Vector2f& startingPosition)
    : velocity(sf::Vector2f(speed * std::sin(angle * M_PI / 180.f),
                            -speed * std::cos(angle * M_PI / 180.f))),
      damage(damage),
      playerProjectile(isPlayer),
      markedForDeletion(false),
      bright(false) {
  setTexture(texture);
  setOrigin(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
  setRotation(angle);
  setPosition(startingPosition);
}

void Projectile::update(float deltaTime) {
  move(velocity * deltaTime);

  if (flashTimer.getElapsedTime().asSeconds() >= 0.01f) {
    flash();
    flashTimer.restart();
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

void Projectile::flash() {
  if (bright)
    setColor(sf::Color(255, 255, 255, 255));
  else
    setColor(sf::Color(255, 255, 255, 200));
  bright = !bright;
}
