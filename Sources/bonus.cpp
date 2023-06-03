#include "bonus.hpp"

Bonus::Bonus(const sf::Texture& texture,
             const bool& powerUp,
             const sf::Vector2f& startingPosition)
    : sf::Sprite(texture),
      powerUp(powerUp),
      velocity(0, 100),
      markedForDeletion(false) {
  if (this->powerUp)
    setTextureRect(
        sf::IntRect(0, 0, texture.getSize().x / 2, texture.getSize().y));
  else
    setTextureRect(sf::IntRect(texture.getSize().x / 2, 0,
                               texture.getSize().x / 2, texture.getSize().y));
  setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
  setPosition(startingPosition);
}

void Bonus::update(const float& deltaTime, sf::RenderWindow& window) {
  move(velocity * deltaTime);
  window.draw(*this);
}

bool Bonus::isMarkedForDeletion() {
  return markedForDeletion;
}

void Bonus::markForDeletion() {
  markedForDeletion = true;
}

bool Bonus::isPowerUp() const {
  return powerUp;
}
