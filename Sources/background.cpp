#include "background.hpp"

Background::Background(const sf::Vector2u& windowSize, sf::Texture& texture)
    : backgroundSprites(2), windowSize(windowSize), scrollSpeed(300) {
  texture.setRepeated(true);

  for (auto& background_sprite : backgroundSprites) {
    background_sprite = std::make_unique<sf::Sprite>(texture);
    background_sprite->setTextureRect({0, 0,
                                       static_cast<int>(this->windowSize.x),
                                       static_cast<int>(this->windowSize.y)});
    background_sprite->setScale(1, 1.1);
  }
  backgroundSprites.back()->setPosition(
      0, -backgroundSprites.back()->getLocalBounds().height);
}

void Background::update(const float& deltaTime, sf::RenderWindow& window) {
  scroll(deltaTime);
  draw(window);
}

void Background::setScrollSpeed(const float& speed) {
  scrollSpeed = speed;
}

void Background::scroll(const float& deltaTime) {
  for (const auto& background_sprite : backgroundSprites) {
    if (background_sprite->getPosition().y > windowSize.y) {
      background_sprite->setPosition(
          0, -background_sprite->getLocalBounds().height);
    }
    background_sprite->move(0, scrollSpeed * deltaTime);
  }
}

void Background::draw(sf::RenderWindow& window) {
  for (const auto& backgroundSprite : backgroundSprites) {
    window.draw(*backgroundSprite);
  }
}
