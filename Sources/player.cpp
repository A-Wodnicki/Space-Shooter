#include "player.hpp"

Player::Player(const sf::Texture& texture,
               const sf::Texture& projectile,
               const sf::Vector2u& windowSize,
               std::vector<std::unique_ptr<Projectile> >& projectiles)
    : Ship(texture,
           sf::Vector2f(windowSize.x / 2, windowSize.y / 4 * 3),
           AppearDirection::Bottom,
           windowSize,
           300,
           projectile,
           projectiles),
      projectileAngle(10),
      hp(3),
      scoreMultiplier(100),
      powerUpCount(3),
      superOnCooldown(true),
      shootingSuper(false) {}

void Player::update(const float& deltaTime, sf::RenderWindow& window) {
  Ship::update(deltaTime);

  sf::Color color =
      getHurtCooldown() < 3 ? sf::Color(255, 255, 255, 150) : sf::Color::White;

  setColor(color);

  shootSuper();

  if (superCooldown.getElapsedTime().asSeconds() >= 3)
    superOnCooldown = false;

  handleControl(deltaTime, window.getSize());

  window.draw(*this);
}

int Player::getHp() const {
  return hp;
}

void Player::setHp(const int& hp) {
  this->hp = hp;
}

float Player::getHurtCooldown() const {
  return hurtCooldown.getElapsedTime().asSeconds();
}

void Player::restartHurtCooldown() {
  hurtCooldown.restart();
}

int Player::getScoreMultiplier() const {
  return scoreMultiplier;
}

int Player::getPowerUpCount() const {
  return powerUpCount;
}

void Player::addPowerUp() {
  powerUpCount++;
}

bool Player::isSuperOnCooldown() const {
  return superOnCooldown;
}

void Player::handleControl(const float& deltaTime,
                           const sf::Vector2u& windowSize) {
  if (appearing)
    return;

  if (sf::Keyboard::isKeyPressed(moveUpButton) &&
      getPosition().y > windowSize.y / 2 + getGlobalBounds().height / 2)
    moveUp(deltaTime);

  if (sf::Keyboard::isKeyPressed(moveRightButton) &&
      getPosition().x < windowSize.x - getGlobalBounds().width / 2)
    moveRight(deltaTime);

  if (sf::Keyboard::isKeyPressed(moveDownButton) &&
      getPosition().y < windowSize.y - getGlobalBounds().height / 2)
    moveDown(deltaTime);

  if (sf::Keyboard::isKeyPressed(moveLeftButton) &&
      getPosition().x > getGlobalBounds().width / 2)
    moveLeft(deltaTime);

  if ((sf::Keyboard::isKeyPressed(moveLeftButton) ==
       sf::Keyboard::isKeyPressed(moveRightButton)) &&
      turning) {
    turning = false;
    changeAnimationType();
  }

  if ((sf::Keyboard::isKeyPressed(moveLeftButton) !=
       sf::Keyboard::isKeyPressed(moveRightButton)) &&
      !turning) {
    turning = true;
    changeAnimationType();
  }

  if (sf::Keyboard::isKeyPressed(shootButton3) && powerUpCount &&
      !shootingSuper && !superOnCooldown) {
    scoreMultiplier = 25;
    shootingSuper = true;
    superDuration.restart();
  }

  if (projectileCooldown.getElapsedTime().asSeconds() < 0.075)
    return;

  projectileCooldown.restart();

  if (sf::Keyboard::isKeyPressed(shootButton1)) {
    scoreMultiplier = 100;
    shoot(0);
  } else if (sf::Keyboard::isKeyPressed(shootButton2)) {
    scoreMultiplier = 50;
    shoot(projectileAngle);

    projectileAngle += 2;
    if (projectileAngle >= 40)
      projectileAngle = 10;
  }
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
  if (!shootingSuper)
    return;

  if (projectileCooldown.getElapsedTime().asSeconds() < 0.075)
    return;

  projectileCooldown.restart();

  shoot(80, 40);

  if (superDuration.getElapsedTime().asSeconds() < 1)
    return;

  shootingSuper = false;
  superCooldown.restart();
  superOnCooldown = true;
  powerUpCount--;
}
