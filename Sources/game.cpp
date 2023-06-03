#include "game.hpp"

Game::Game()
    : window(sf::VideoMode::getDesktopMode(),
             "Space Shooter",
             sf::Style::Fullscreen),
      texturesDirectory("assets/images/"),
      textureFileExtension(".png"),
      scoreValue(0),
      playerHp("", font),
      playerPowerUpCount("", font),
      score("", font) {
  ::ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);
  window.setFramerateLimit(144);

  loadTexture("background", "blue");
  loadTexture("bonus", "bonus");
  loadTexture("projectile", "laserBlue01");
  loadTexture("player", "player");
  loadTexture("explosion", "explosion");
  loadTexture("enemy", "enemy1");

  for (auto& [key, value] : textures) {
    value->setSmooth(true);
  }

  background =
      std::make_unique<Background>(window.getSize(), *textures["background"]);

  player =
      std::make_unique<Player>(*textures["player"], *textures["projectile"],
                               window.getSize(), projectiles);

  font.loadFromFile("assets/fonts/kenvector_future.ttf");

  score.setPosition(2 * score.getCharacterSize(), 2 * score.getCharacterSize());
  playerHp.setPosition(2 * playerHp.getCharacterSize(),
                       3 * playerHp.getCharacterSize());
  playerPowerUpCount.setPosition(2 * playerPowerUpCount.getCharacterSize(),
                                 4 * playerPowerUpCount.getCharacterSize());

  numbersBox.setPosition(score.getPosition() / 2.f);
  numbersBox.setFillColor(sf::Color::Transparent);
  numbersBox.setOutlineColor(sf::Color::White);
  numbersBox.setOutlineThickness(-5);
  numbersBox.setSize(
      sf::Vector2f(0, playerPowerUpCount.getPosition().y +
                          playerPowerUpCount.getCharacterSize()));
}

void Game::loop() {
  enemies.emplace_back(std::make_unique<Enemy>(
      *textures["enemy"], sf::Vector2f(300.f, 300.f), AppearDirection::Right,
      20, window.getSize(), *textures["projectile"], projectiles));
  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();

    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();

    background->update(deltaTime, window);

    updateEnemies(deltaTime);

    updateBonuses(deltaTime);

    player->update(deltaTime, window);

    updateProjectiles(deltaTime);

    updateUi();

    window.display();
  }
}

void Game::loadTexture(const std::string& textureName,
                       const std::string& fileName) {
  auto texture = std::make_unique<sf::Texture>();
  if (!texture->loadFromFile(texturesDirectory + fileName +
                             textureFileExtension))
    window.close();
  textures[textureName] = std::move(texture);
}

void Game::updateEnemies(const float& deltaTime) {
  enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                               [](const std::unique_ptr<Enemy>& enemy) {
                                 return enemy->isMarkedForDeletion();
                               }),
                enemies.end());

  enemies.shrink_to_fit();

  for (auto& enemy : enemies) {
    if (!enemy->getHp()) {
      enemy->markForDeletion();
      scoreValue += 1 * player->getScoreMultiplier();
    }

    enemy->update(deltaTime, window);
  }
}

void Game::updateBonuses(const float& deltaTime) {
  bonuses.erase(std::remove_if(bonuses.begin(), bonuses.end(),
                               [](const std::unique_ptr<Bonus>& bonus) {
                                 return bonus->isMarkedForDeletion();
                               }),
                bonuses.end());

  bonuses.shrink_to_fit();

  sf::FloatRect playerBounds = player->getGlobalBounds();

  for (auto& bonus : bonuses) {
    sf::FloatRect bonusBounds = bonus->getGlobalBounds();

    if (bonusBounds.intersects(playerBounds))
      bonus->isPowerUp() ? player->addPowerUp()
                         : player->setHp(player->getHp() + 1);

    if (bonusBounds.intersects(playerBounds) ||
        bonusBounds.top - bonusBounds.height > window.getSize().y)
      bonus->markForDeletion();

    bonus->update(deltaTime, window);
  }
}

void Game::updateProjectiles(const float& deltaTime) {
  projectiles.erase(
      std::remove_if(projectiles.begin(), projectiles.end(),
                     [](const std::unique_ptr<Projectile>& projectile) {
                       return projectile->isMarkedMarkedForDeletion();
                     }),
      projectiles.end());

  projectiles.shrink_to_fit();

  sf::FloatRect playerBounds = player->getGlobalBounds();

  for (auto& projectile : projectiles) {
    sf::FloatRect projectileBounds = projectile->getGlobalBounds();

    if (projectileBounds.top < -projectileBounds.height ||
        projectileBounds.left < -projectileBounds.width ||
        projectileBounds.left - projectileBounds.width > window.getSize().x ||
        projectileBounds.top - projectileBounds.height > window.getSize().y)
      projectile->markForDeletion();

    if (playerBounds.contains(projectile->getPosition()) &&
        !projectile->isPlayerOwned()) {
      projectile->markForDeletion();
      player->setHp(player->getHp() - 1);
    }

    if (projectile->isPlayerOwned())
      for (auto& enemy : enemies) {
        sf::FloatRect enemyBounds = enemy->getGlobalBounds();
        if (enemyBounds.intersects(projectileBounds)) {
          projectile->markForDeletion();
          enemy->setHp(enemy->getHp() - 1);
        }
      }

    projectile->update(deltaTime, window);
  }
}

void Game::updateUi() {
  score.setString("score: " + std::to_string(scoreValue));
  playerHp.setString("hp: " + std::to_string(player->getHp()));
  playerPowerUpCount.setString("special: " +
                               std::to_string(player->getPowerUpCount()));

  numbersBox.setSize(sf::Vector2f(
      std::max({score.getGlobalBounds().width, playerHp.getGlobalBounds().width,
                playerPowerUpCount.getGlobalBounds().width}) +
          2 * score.getCharacterSize(),
      numbersBox.getSize().y));

  if (player->isSuperOnCooldown())
    playerPowerUpCount.setFillColor(sf::Color(255, 255, 255, 100));
  else
    playerPowerUpCount.setFillColor(sf::Color(255, 255, 255, 255));

  window.draw(numbersBox);
  window.draw(score);
  window.draw(playerHp);
  window.draw(playerPowerUpCount);
}
