#include "game.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(),
             "Space Shooter",
             sf::Style::Fullscreen),
      gameState(GameState::Playing),
      texturesDirectory("assets/images/"),
      textureFileExtension(".png"),
      textureNames{"background",      "bonus",  "projectile",
                   "enemyProjectile", "player", "explosion"},
      textureFileNames{"blue",       "bonus",  "laserBlue01",
                       "laserRed01", "player", "explosion"},
      scoreValue(0),
      playerHp("", font),
      playerPowerUpCount("", font),
      score("", font),
      line(sf::Lines, 2),
      gen(randomDevice()),
      randomEnemy(0, 5),
      randomEnemyPositionX(50, window.getSize().x - 50),
      randomEnemyPositionY(50, window.getSize().y / 2 - 50),
      chance(0, 1),
      namePrompt("Enter your name and press space", font),
      nameInput("", font),
      positionText("", font),
      nameText("", font),
      scoreText("", font),
      exitPrompt("Press space to exit", font) {
  window.setFramerateLimit(144);
  window.setKeyRepeatEnabled(false);

  for (int i = 0; i < 6; ++i) {
    textureNames.emplace_back("enemy" + std::to_string(i));
    textureFileNames.emplace_back("enemy" + std::to_string(i + 1));
  }

  loadTextures();

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

  line[0].position = sf::Vector2f(0, window.getSize().y / 2);
  line[1].position = sf::Vector2f(window.getSize().x, window.getSize().y / 2);

  namePrompt.setOrigin(namePrompt.getGlobalBounds().width / 2, 0);
  nameInput.setPosition(window.getSize().x / 2, window.getSize().y / 2);
  namePrompt.setPosition(window.getSize().x / 2,
                         nameInput.getGlobalBounds().top -
                             (2 * namePrompt.getGlobalBounds().height));

  exitPrompt.setOrigin(exitPrompt.getGlobalBounds().width / 2,
                       exitPrompt.getGlobalBounds().height / 2);
  exitPrompt.setPosition(window.getSize().x / 2, 3 * window.getSize().y / 4);
}

void Game::loop() {
  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();

    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::TextEntered) {
        if (gameState == GameState::GameOver) {
          if (std::isalpha(event.text.unicode) &&
              nameInput.getString().getSize() < 3) {
            nameInput.setString(nameInput.getString() + event.text.unicode);
          } else if (event.text.unicode == 8 &&
                     !nameInput.getString().isEmpty()) {
            std::string newInput = nameInput.getString();
            newInput.pop_back();
            nameInput.setString(newInput);
          }
          nameInput.setPosition(
              window.getSize().x / 2 - nameInput.getGlobalBounds().width / 2,
              nameInput.getPosition().y);
        }
      }

      if (event.type == sf::Event::KeyPressed)
        if (event.key.code == sf::Keyboard::Space) {
          if (gameState == GameState::Scores)
            window.close();
          if (gameState == GameState::GameOver &&
              nameInput.getString().getSize() == 3) {
            saveScore();
            gameState = GameState::Scores;

            std::vector<ScoreEntry> scores = readScores();

            for (size_t i = 0; i < scores.size(); ++i) {
              positionText.setString(positionText.getString() +
                                     std::to_string(i + 1) + ".\n");
              nameText.setString(nameText.getString() + scores[i].name + "\n");
              scoreText.setString(scoreText.getString() +
                                  std::to_string(scores[i].score) + "\n");
            }

            nameText.setPosition(positionText.getGlobalBounds().width, 0);
            scoreText.setPosition(positionText.getGlobalBounds().width +
                                      scoreText.getGlobalBounds().width,
                                  0);

            float totalWidth = positionText.getGlobalBounds().width +
                               nameText.getGlobalBounds().width +
                               scoreText.getGlobalBounds().width;

            float centerX = (window.getSize().x - totalWidth) / 2;
            float centerY =
                (window.getSize().y - positionText.getGlobalBounds().height) /
                2;

            positionText.move(centerX, centerY);
            nameText.move(centerX + positionText.getGlobalBounds().width,
                          centerY);
            scoreText.move(centerX + positionText.getGlobalBounds().width +
                               nameText.getGlobalBounds().width,
                           centerY);
          }
        }
    }

    window.clear();

    background->update(deltaTime, window);

    updateEnemies(deltaTime);

    updateExplosions();

    updateBonuses(deltaTime);

    updateProjectiles(deltaTime);

    switch (gameState) {
      case GameState::Playing:

        spawnEnemies();

        player->update(deltaTime, window);

        updateUi();

        if (!player->getHp()) {
          explosions.emplace_back(std::make_unique<Explosion>(
              *textures["explosion"], player->getPosition()));

          gameState = GameState::GameOver;
        }

        break;
      case GameState::GameOver:
        window.draw(namePrompt);
        window.draw(nameInput);
        break;
      case GameState::Scores:
        window.draw(positionText);
        window.draw(nameText);
        window.draw(scoreText);
        window.draw(exitPrompt);
        break;
    }

    window.display();
  }
}

void Game::loadTextures() {
  if (textureNames.size() != textureFileNames.size())
    window.close();

  for (size_t i = 0; i < textureNames.size(); ++i) {
    textures[textureNames[i]] = std::make_unique<sf::Texture>();

    if (!textures[textureNames[i]]->loadFromFile(
            texturesDirectory + textureFileNames[i] + textureFileExtension))
      window.close();
  }
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

      if (chance(gen) < 0.1)
        bonuses.emplace_back(std::make_unique<Bonus>(
            *textures["bonus"], chance(gen) < 0.5 ? true : false,
            enemy->getPosition()));

      explosions.emplace_back(std::make_unique<Explosion>(
          *textures["explosion"], enemy->getPosition()));
    }

    enemy->update(deltaTime, window, player->getPosition());
  }
}

void Game::updateExplosions() {
  removeMarkedObjects(explosions,
                      [](const std::unique_ptr<Explosion>& explosion) {
                        return explosion->isMarkedForDeletion();
                      });

  for (auto& explosion : explosions) {
    explosion->update(window);
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

  for (size_t i = 0; i < line.getVertexCount(); ++i) {
    line[i].color.a =
        255 - 255 * ((player->getGlobalBounds().top +
                      player->getGlobalBounds().height - line.getBounds().top) /
                     (line.getBounds().top + player->getGlobalBounds().height));
  }

  window.draw(line);
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

void Game::saveScore() {
  std::vector<ScoreEntry> scores = readScores();

  scores.emplace_back(ScoreEntry{nameInput.getString(), scoreValue});

  std::sort(scores.begin(), scores.end(),
            [](const ScoreEntry& a, const ScoreEntry& b) {
              return a.score > b.score;
            });

  if (scores.size() > 10)
    scores.resize(10);

  std::ofstream file("scores.txt");
  if (file.is_open()) {
    for (const auto& entry : scores) {
      file << entry.name << " " << entry.score << "\n";
    }
    file.close();
  } else
    std::cerr << "Could not open file\n";
}

std::vector<ScoreEntry> Game::readScores() {
  std::vector<ScoreEntry> scores;

  std::ifstream file("scores.txt");
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      std::string name;
      int score;
      if (iss >> name >> score)
        scores.emplace_back(ScoreEntry{name, score});
    }
    file.close();
  } else
    std::cerr << "Could not open file\n";

  return scores;
}
