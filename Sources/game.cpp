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
      score("", font),
      gen(randomDevice()),
      randomEnemy(0, 5),
      randomEnemyPositionX(0, window.getSize().x),
      randomEnemyPositionY(0, window.getSize().y / 2) {
  ::ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);
  window.setFramerateLimit(144);

  loadTexture("background", "blue");
  loadTexture("bonus", "bonus");
  loadTexture("projectile", "laserBlue01");
  loadTexture("enemyProjectile", "laserRed01");
  loadTexture("player", "player");
  loadTexture("explosion", "explosion");
  for (int i = 0; i < 6; ++i)
    loadTexture("enemy" + std::to_string(i), "enemy" + std::to_string(i + 1));

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
  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();

    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();

    background->update(deltaTime, window);

    spawnEnemies();

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
  textures[textureName] = std::make_unique<sf::Texture>();

  if (!textures[textureName]->loadFromFile(texturesDirectory + fileName +
                                           textureFileExtension))
    window.close();
}

template <typename T, typename Predicate>
void Game::removeMarkedObjects(std::vector<T>& objects, Predicate pred) {
  objects.erase(std::remove_if(objects.begin(), objects.end(), pred),
                objects.end());
  objects.shrink_to_fit();
}

void Game::updateEnemies(const float& deltaTime) {
  removeMarkedObjects(enemies, [](const std::unique_ptr<Enemy>& enemy) {
    return enemy->isMarkedForDeletion();
  });

  for (auto& enemy : enemies) {
    if (!enemy->getHp()) {
      enemy->markForDeletion();
      scoreValue += 1 * player->getScoreMultiplier();
    }

    enemy->update(deltaTime, window, player->getPosition());
  }
}

void Game::updateBonuses(const float& deltaTime) {
  removeMarkedObjects(bonuses, [](const std::unique_ptr<Bonus>& bonus) {
    return bonus->isMarkedForDeletion();
  });

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
  removeMarkedObjects(projectiles,
                      [](const std::unique_ptr<Projectile>& projectile) {
                        return projectile->isMarkedMarkedForDeletion();
                      });

  sf::FloatRect playerBounds = player->getGlobalBounds();

  for (auto& projectile : projectiles) {
    sf::FloatRect projectileBounds = projectile->getGlobalBounds();

    if (projectileBounds.top < -projectileBounds.height ||
        projectileBounds.left < -projectileBounds.width ||
        projectileBounds.left - projectileBounds.width > window.getSize().x ||
        projectileBounds.top - projectileBounds.height > window.getSize().y)
      projectile->markForDeletion();

    if (playerBounds.contains(projectile->getPosition()) &&
        !projectile->isPlayerOwned() && player->getHurtCooldown() >= 3) {
      projectile->markForDeletion();
      player->setHp(player->getHp() - 1);
      player->restartHurtCooldown();
    }

    if (projectile->isPlayerOwned())
      for (auto& enemy : enemies) {
        sf::FloatRect enemyBounds = enemy->getGlobalBounds();
        if (enemyBounds.intersects(projectileBounds)) {
          projectile->markForDeletion();
          enemy->getHp() < 0 ? enemy->setHp(0)
                             : enemy->setHp(enemy->getHp() - 1);
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
    playerPowerUpCount.setFillColor(sf::Color::White);

  if (player->getHurtCooldown() < 3)
    playerHp.setFillColor(sf::Color(255, 255, 255, 100));
  else
    playerHp.setFillColor(sf::Color::White);

  window.draw(numbersBox);
  window.draw(score);
  window.draw(playerHp);
  window.draw(playerPowerUpCount);
}

void Game::spawnEnemies() {
  if (enemies.size())
    enemySpawnCooldown.restart();

  if (enemies.size() || enemySpawnCooldown.getElapsedTime().asSeconds() < 3)
    return;

  for (int i = 0; i < 5; ++i) {
    int enemy = randomEnemy(gen);

    sf::Vector2f enemyPosition(randomEnemyPositionX(gen),
                               randomEnemyPositionY(gen));

    AppearDirection enemyAppearDirection;

    if (enemyPosition.y <= window.getSize().y / 4)
      enemyAppearDirection = AppearDirection::Top;
    else if (enemyPosition.x <= window.getSize().x / 2)
      enemyAppearDirection = AppearDirection::Left;
    else
      enemyAppearDirection = AppearDirection::Right;

    enemies.emplace_back(std::make_unique<Enemy>(
        *textures["enemy" + std::to_string(enemy)], enemyPosition,
        enemyAppearDirection, 20, window.getSize(),
        *textures["enemyProjectile"], projectiles, enemy));
  }
}
