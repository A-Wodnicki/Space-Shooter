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
      projectile(projectile),
      hp(3),
      powerUpCount(3),
      shootingSuper(false) {}

void Player::update(const float& deltaTime) {
  Ship::update(deltaTime);

  if (shootingSuper)
    shootSuper();

  handleControl(deltaTime);
}

int Player::getHp() const {
  return hp;
}

int Player::getPowerUpCount() const {
  return powerUpCount;
}

void Player::addHp() {
  hp++;
}

void Player::addPowerUp() {
  powerUpCount++;
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

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L) && powerUpCount &&
      !shootingSuper && superCooldown.getElapsedTime().asSeconds() > 3) {
    powerUpCount--;
    shootingSuper = true;
    superDuration.restart();
  }

  if (sf::Keyboard::isKeyPressed(shootButton1) &&
      projectileCooldown.getElapsedTime().asSeconds() >= 0.075) {
    shoot(0);
  }

  if (sf::Keyboard::isKeyPressed(shootButton2) &&
      projectileCooldown.getElapsedTime().asSeconds() >= 0.075) {
    shoot(projectileAngle);
    projectileAngle += 2;
    if (projectileAngle >= 40)
      projectileAngle = 10;
  }

  if (projectileCooldown.getElapsedTime().asSeconds() >= 0.075)
    projectileCooldown.restart();
}

void Player::shoot(int angle, int projectilesToShoot) {
  for (int i = 0; i < projectilesToShoot; ++i)
    projectiles.emplace_back(std::make_unique<Projectile>(
        projectile, 1000,
        !angle
            ? 0
            : 360 - angle % 90 +
                  2 * i * (360 - (360 - angle % 90)) / (projectilesToShoot - 1),
        true,
        sf::Vector2f(getGlobalBounds().left +
                         getGlobalBounds().width / (projectilesToShoot - 1) * i,
                     getGlobalBounds().top)));
}

void Player::shootSuper() {
  if (superDuration.getElapsedTime().asSeconds() >= 1) {
    shootingSuper = false;
    superCooldown.restart();
  }

  if (projectileCooldown.getElapsedTime().asSeconds() < 0.075)
    return;

  shoot(80, 40);
  projectileCooldown.restart();
}
