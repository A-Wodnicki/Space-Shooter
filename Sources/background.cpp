#include "background.hpp"

Background::Background(const sf::Vector2u& windowSize, sf::Texture& texture)
    : backgroundSprites(2), windowSize(windowSize), scrollSpeed(300) {
  for (auto& background_sprite : backgroundSprites) {
    background_sprite = std::make_unique<sf::Sprite>(texture);
    background_sprite->setTextureRect(
        sf::IntRect(0, 0, this->windowSize.x, this->windowSize.y));
    background_sprite->setScale(1, 1.1);
  }

  texture.setRepeated(true);

  backgroundSprites.back()->setPosition(
      0, -backgroundSprites.back()->getLocalBounds().height);
}

void Background::update(const float& deltaTime, sf::RenderWindow& window) {
  scroll(deltaTime);
  draw(window);
}

void Background::scroll(const float& deltaTime) {
  for (const auto& background_sprite : backgroundSprites) {
    background_sprite->move(0, scrollSpeed * deltaTime);

    if (background_sprite->getPosition().y > windowSize.y)
      background_sprite->setPosition(
          0, -background_sprite->getLocalBounds().height);
  }
}

void Background::draw(sf::RenderWindow& window) {
  for (const auto& background_sprite : backgroundSprites)
    window.draw(*background_sprite);
}
