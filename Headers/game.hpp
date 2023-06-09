#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
#include "background.hpp"
#include "bonus.hpp"
#include "enemy.hpp"
#include "explosion.hpp"
#include "player.hpp"
#include "projectile.hpp"

enum class GameState { Playing, GameOver, Scores };

struct ScoreEntry {
  std::string name;
  int score;
};

class Game {
 public:
  Game();

  void loop();

 private:
  void loadTextures();

  template <typename T, typename Predicate>
  void removeMarkedObjects(std::vector<T>& objects, Predicate pred);

  void updateEnemies(const float& deltaTime);
  void updateExplosions();
  void updateBonuses(const float& deltaTime);
  void updateProjectiles(const float& deltaTime);

  void updateUi();

  void spawnEnemies();

  void saveScore();
  std::vector<ScoreEntry> readScores();

  sf::RenderWindow window;
  sf::Clock clock;
  GameState gameState;

  std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
  std::string texturesDirectory;
  std::string textureFileExtension;
  std::vector<std::string> textureNames;
  std::vector<std::string> textureFileNames;

  std::unique_ptr<Background> background;

  std::vector<std::unique_ptr<Projectile>> projectiles;
  std::vector<std::unique_ptr<Bonus>> bonuses;
  std::vector<std::unique_ptr<Enemy>> enemies;
  std::vector<std::unique_ptr<Explosion>> explosions;

  std::unique_ptr<Player> player;

  int scoreValue;

  sf::Font font;
  sf::Text playerHp;
  sf::Text playerPowerUpCount;
  sf::Text score;
  sf::RectangleShape numbersBox;
  sf::VertexArray line;

  std::random_device randomDevice;
  std::mt19937 gen;
  std::uniform_int_distribution<int> randomEnemy;
  std::uniform_real_distribution<float> randomEnemyPositionX;
  std::uniform_real_distribution<float> randomEnemyPositionY;
  sf::Clock enemySpawnCooldown;

  std::uniform_real_distribution<float> chance;

  sf::Text namePrompt;
  sf::Text nameInput;

  sf::Text positionText;
  sf::Text nameText;
  sf::Text scoreText;
  sf::Text exitPrompt;
};

#endif  // GAME_HPP
