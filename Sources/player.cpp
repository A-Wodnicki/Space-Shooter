#include "player.hpp"

Player::Player(const sf::Texture& texture,
               const sf::Texture& projectile,
               const sf::Vector2u& windowSize,
               std::vector<std::unique_ptr<Projectile> >& projectiles)
    : Ship(texture,
           {static_cast<float>(windowSize.x / 2),
            static_cast<float>(windowSize.y / 4 * 3)},
           AppearDirection::Bottom,
           windowSize,
           300,
           projectiles),
      projectile(projectile) {}

void Player::update(const float& deltaTime) {
  handleControl(deltaTime);
  Ship::update(deltaTime);
}

void Player::handleControl(const float& deltaTime) {
  if (isAppearing)
    return;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    moveUp(deltaTime);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    moveRight(deltaTime);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    moveDown(deltaTime);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    moveLeft(deltaTime);

  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ==
       sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) &&
      isTurning) {
    isTurning = false;
    changeAnimationType();
  }

  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) !=
       sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) &&
      !isTurning) {
    isTurning = true;
    changeAnimationType();
  }

  if (sf::Keyboard::isKeyPressed(shootButton) &&
      projectileCooldown.getElapsedTime().asSeconds() >= .1) {
    projectileCooldown.restart();
    for (int i = 0; i < 4; ++i)
      projectiles.emplace_back(std::make_unique<Projectile>(
          projectile, 500, 345 + i * 10, 1, true,
          sf::Vector2f(getPosition().x, getGlobalBounds().top)));
  }
}
