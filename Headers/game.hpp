#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include <windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "background.hpp"
#include "bonus.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "projectile.hpp"

class Game {
 public:
  Game();

  void loop();

 private:
  void loadTexture(const std::string& textureName, const std::string& fileName);
  void updateEnemies(const float& deltaTime);
  void updateBonuses(const float& deltaTime);
  void updateProjectiles(const float& deltaTime);
  void updateUi();

  sf::RenderWindow window;
  sf::Clock clock;

  std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
  std::string texturesDirectory;
  std::string textureFileExtension;

  std::unique_ptr<Background> background;

  std::vector<std::unique_ptr<Projectile>> projectiles;
  std::vector<std::unique_ptr<Bonus>> bonuses;
  std::vector<std::unique_ptr<Enemy>> enemies;

  std::unique_ptr<Player> player;

  int scoreValue;

  sf::Font font;
  sf::Text playerHp;
  sf::Text playerPowerUpCount;
  sf::Text score;
  sf::RectangleShape numbersBox;
};

#endif  // GAMEENGINE_HPP
