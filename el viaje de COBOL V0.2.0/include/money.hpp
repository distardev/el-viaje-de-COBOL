#ifndef MONEY_HPP
#define MONEY_HPP

#include <SFML/Graphics.hpp>
#include <audio_manager.hpp>

class Money {
public:
    Money(sf::Vector2f pos, const sf::Texture& texture, AudioManager* audioManager);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool checkCollision(const sf::FloatRect& playerBounds);
    bool isCollected() const;

private:
    sf::Sprite sprite;
    sf::Vector2f position;
    float animationTimer = 0.f;
    int currentFrame = 0;

    bool collected = false;

    AudioManager* audio = nullptr;
};

#endif
