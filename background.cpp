#include "background.h"

Background::Background(sf::RenderWindow& window, const sf::Texture& texture) {
  background_sprites.emplace_back(std::make_unique<sf::Sprite>());
  background_sprites.emplace_back(std::make_unique<sf::Sprite>());

  for (auto& background_sprite : background_sprites) {
    background_sprite->setTexture(texture);
    background_sprite->setTextureRect({0, 0,
                                       static_cast<int>(window.getSize().x),
                                       static_cast<int>(window.getSize().y)});
    background_sprite->setScale(1, 1.1);
  }
  background_sprites.back()->setPosition(
      0, -background_sprites.back()->getLocalBounds().height);
}

void Background::scroll(sf::RenderWindow& window, const sf::Time& elapsed) {
  for (auto& background_sprite : background_sprites) {
    if (background_sprite->getPosition().y > window.getSize().y) {
      background_sprite->setPosition(
          0, -background_sprite->getLocalBounds().height);
    }
    background_sprite->move(0, scroll_speed * elapsed.asSeconds());
    window.draw(*background_sprite);
  }
}

void Background::set_scroll_speed(const float& speed) {
  scroll_speed = speed;
}
