#include "game.hpp"
#include "player.hpp"

Game::Game()
    : window(sf::VideoMode::getDesktopMode(),
             "Space Shooter",
             sf::Style::Close),
      texturesDirectory("assets/images/"),
      textureFileExtension(".png") {
  ::ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);
  window.setFramerateLimit(144);
  window.setKeyRepeatEnabled(false);

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
}

void Game::loop() {
  auto player =
      std::make_unique<Player>(*textures["player"], *textures["projectile"],
                               window.getSize(), projectiles);
  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();

    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();

    background->update(deltaTime, window);

    updateBonuses(deltaTime);

    player->update(deltaTime);
    window.draw(*player);

    updateProjectiles(deltaTime);

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
  for (auto& projectile : projectiles) {
    sf::FloatRect projectileBounds = projectile->getGlobalBounds();
    if (projectileBounds.top < -projectileBounds.height ||
        projectileBounds.left < -projectileBounds.width ||
        projectileBounds.left - projectileBounds.width > window.getSize().x ||
        projectileBounds.top - projectileBounds.height > window.getSize().y)
      projectile->markForDeletion();
    projectile->update(deltaTime);
    window.draw(*projectile);
  }
}

void Game::updateBonuses(const float& deltaTime) {
  bonuses.erase(std::remove_if(bonuses.begin(), bonuses.end(),
                               [](const std::unique_ptr<Bonus>& bonus) {
                                 return bonus->isMarkedForDeletion();
                               }),
                bonuses.end());
  bonuses.shrink_to_fit();
  for (auto& bonus : bonuses) {
    sf::FloatRect bonusBounds = bonus->getGlobalBounds();
    if (bonusBounds.top - bonusBounds.height > window.getSize().y)
      bonus->markForDeletion();
    bonus->update(deltaTime);
    window.draw(*bonus);
  }
}
