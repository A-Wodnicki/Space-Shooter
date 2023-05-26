#include "background.hpp"

Background::Background(const sf::Vector2u& windowSize, sf::Texture& texture)
    : windowSize(windowSize), scrollSpeed(300) {
  texture.setRepeated(true);

  backgroundSprites.emplace_back(std::make_unique<sf::Sprite>());
  backgroundSprites.emplace_back(std::make_unique<sf::Sprite>());

  for (auto& background_sprite : backgroundSprites) {
    background_sprite->setTexture(texture);
    background_sprite->setTextureRect({0, 0,
                                       static_cast<int>(this->windowSize.x),
                                       static_cast<int>(this->windowSize.y)});
    background_sprite->setScale(1, 1.1);
  }
  backgroundSprites.back()->setPosition(
      0, -backgroundSprites.back()->getLocalBounds().height);
}

void Background::update(const sf::Time& elapsed, sf::RenderWindow& window) {
  scroll(elapsed);
  draw(window);
}

void Background::setScrollSpeed(const float& speed) {
  scrollSpeed = speed;
}

void Background::scroll(const sf::Time& elapsed) {
  for (const auto& background_sprite : backgroundSprites) {
    if (background_sprite->getPosition().y > windowSize.y) {
      background_sprite->setPosition(
          0, -background_sprite->getLocalBounds().height);
    }
    background_sprite->move(0, scrollSpeed * elapsed.asSeconds());
  }
}

void Background::draw(sf::RenderWindow& window) {
  for (const auto& backgroundSprite : backgroundSprites) {
    window.draw(*backgroundSprite);
  }
}
