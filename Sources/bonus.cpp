#include "bonus.hpp"

Bonus::Bonus(const sf::Texture& texture,
             const bool& isPowerUp,
             const sf::Vector2f& startingPosition)
    : isPowerUp(isPowerUp), velocity(0, 100), markedForDeletion(false) {
  setTexture(texture);
  if (this->isPowerUp)
    setTextureRect(
        sf::IntRect(0, 0, texture.getSize().x / 2, texture.getSize().y));
  else
    setTextureRect(sf::IntRect(texture.getSize().x / 2, 0,
                               texture.getSize().x / 2, texture.getSize().y));
  setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
  setPosition(startingPosition);
}

void Bonus::update(const float& deltaTime) {
  move(velocity * deltaTime);
}

bool Bonus::isMarkedForDeletion() {
  return markedForDeletion;
}

void Bonus::markForDeletion() {
  markedForDeletion = true;
}
