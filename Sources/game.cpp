#include "game.hpp"
#include <iostream>
#include "projectile.hpp"

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "Space Shooter", sf::Style::None),
      texturesDirectory("assets/images/"),
      textureFileExtension(".png") {
  ::ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);
  window.setFramerateLimit(144);

  loadTexture("background", "blue");
  loadTexture("projectile", "laserBlue01");

  for (auto& [key, value] : textures) {
    value->setSmooth(true);
  }

  background =
      std::make_unique<Background>(window.getSize(), *textures["background"]);
}

void Game::loop() {
  while (window.isOpen()) {
    sf::Time elapsed = clock.restart();

    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();

    background->update(elapsed, window);

    updateProjectiles(elapsed);

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

void Game::updateProjectiles(const sf::Time& elapsed) {
  projectiles.erase(
      std::remove_if(projectiles.begin(), projectiles.end(),
                     [](const std::unique_ptr<Projectile>& projectile) {
                       return projectile->isMarkedMarkedForDeletion();
                     }),
      projectiles.end());
  projectiles.shrink_to_fit();
  for (auto& projectile : projectiles) {
    sf::Vector2f projectilePosition = projectile->getPosition();
    if (projectilePosition.x < 0 || projectilePosition.y < 0 ||
        projectilePosition.x > window.getSize().x ||
        projectilePosition.y > window.getSize().y)
      projectile->markForDeletion();
    projectile->update(elapsed.asSeconds());
    window.draw(*projectile);
  }
}
