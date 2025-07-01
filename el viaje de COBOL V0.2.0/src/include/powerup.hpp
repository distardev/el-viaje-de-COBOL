#pragma once
#include <SFML/Graphics.hpp>

class PowerUp {
public:
    PowerUp();

    void spawnRandomly(const sf::FloatRect& bounds, sf::Texture& texture);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isActive() const;
    void deactivate();

    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
    sf::Texture* texturePtr;

    float animationTimer;
    float animationSpeed;
    int currentFrame;

    bool active;

    float lifetime = 0.f;
    float maxLifetime = 5.f; 
    float blinkStart = 4.f; 
};
