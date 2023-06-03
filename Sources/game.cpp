#include "game.hpp"

Game::Game()
    : window(sf::VideoMode::getDesktopMode(),
             "Space Shooter",
             sf::Style::Fullscreen),
      texturesDirectory("assets/images/"),
      textureFileExtension(".png"),
      playerHp("", font),
      playerPowerUpCount("", font),
      score("score: 0", font),
      numbersBox() {
  ::ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);
  window.setFramerateLimit(144);

  loadTexture("background", "blue");
  loadTexture("bonus", "bonus");
  loadTexture("projectile", "laserBlue01");
  loadTexture("player", "player");
  loadTexture("explosion", "explosion");

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

    playerHp.setString("hp: " + std::to_string(player->getHp()));
    playerPowerUpCount.setString("special: " +
                                 std::to_string(player->getPowerUpCount()));

    numbersBox.setSize(
        sf::Vector2f(std::max({score.getGlobalBounds().width,
                               playerHp.getGlobalBounds().width,
                               playerPowerUpCount.getGlobalBounds().width}) +
                         2 * score.getCharacterSize(),
                     numbersBox.getSize().y));

    if (player->isSuperOnCooldown())
      playerPowerUpCount.setFillColor(sf::Color(255, 255, 255, 100));
    else
      playerPowerUpCount.setFillColor(sf::Color(255, 255, 255, 255));

    window.clear();

    background->update(deltaTime, window);

    updateBonuses(deltaTime);

    player->update(deltaTime, window);

    updateProjectiles(deltaTime);

    window.draw(numbersBox);
    window.draw(playerHp);
    window.draw(playerPowerUpCount);
    window.draw(score);

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

    projectile->update(deltaTime, window);
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
