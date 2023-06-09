#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include <SFML/Graphics.hpp>

class Explosion : public sf::Sprite {
 public:
  Explosion(const sf::Texture& texture, const sf::Vector2f position);

  void update(sf::RenderWindow& window);

  bool isMarkedForDeletion() const;

 private:
  void animate();

  int frames;
  std::vector<sf::IntRect> animationFrames;
  std::vector<sf::IntRect>::iterator currentFrame;
  sf::Clock animationTimer;

  bool markedForDeletion;
};

#endif  // EXPLOSION_HPP
