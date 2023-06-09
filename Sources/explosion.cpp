#include "explosion.hpp"

Explosion::Explosion(const sf::Texture& texture, const sf::Vector2f position)
    : Sprite(texture),
      frames(5),
      animationFrames(frames),
      markedForDeletion(false) {
  for (int i = 0; i < frames; ++i) {
    animationFrames[i] =
        sf::IntRect(i * getGlobalBounds().width / frames, 0,
                    getGlobalBounds().width / frames, getGlobalBounds().height);
  }

  currentFrame = animationFrames.begin();

  setTextureRect(*currentFrame);

  setOrigin(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height) *
            0.5f);
  setScale(getScale() * 3.f);
  setPosition(position);
}

void Explosion::update(sf::RenderWindow& window) {
  animate();
  window.draw(*this);
}

bool Explosion::isMarkedForDeletion() const {
  return markedForDeletion;
}

void Explosion::animate() {
  if (animationTimer.getElapsedTime().asSeconds() < 1.f / frames)
    return;

  animationTimer.restart();

  currentFrame++;

  if (currentFrame != animationFrames.end())
    setTextureRect(*currentFrame);
  else
    markedForDeletion = true;
}
