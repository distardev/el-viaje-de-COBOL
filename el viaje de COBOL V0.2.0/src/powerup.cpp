#include "powerup.hpp"

PowerUp::PowerUp()
    : texturePtr(nullptr), animationTimer(0.f), animationSpeed(0.5f), currentFrame(0), active(false)
{
}

void PowerUp::spawnRandomly(const sf::FloatRect& bounds, sf::Texture& texture) {
    texturePtr = &texture;
    sprite.setTexture(texture);
    sprite.setOrigin(8, 8);

    float x = bounds.left + static_cast<float>(rand()) / RAND_MAX * bounds.width;
    float y = bounds.top + static_cast<float>(rand()) / RAND_MAX * bounds.height;
    sprite.setPosition(x, y);

    currentFrame = 0;
    animationTimer = 0.f;
    active = true;

    sprite.setTextureRect(sf::IntRect(currentFrame * 16, 0, 16, 16));
}

void PowerUp::update(float dt) {
    if (!active) return;

    animationTimer += dt;
    if (animationTimer >= animationSpeed) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % 2;
        sprite.setTextureRect(sf::IntRect(currentFrame * 16, 0, 16, 16));
    }

    lifetime += dt;
    if (lifetime >= maxLifetime) {
        active = false; 
        return;
    }

    if (lifetime >= blinkStart) {
        float blinkPeriod = 0.2f;
        int blinkPhase = static_cast<int>((lifetime - blinkStart) / blinkPeriod) % 2;
        if (blinkPhase == 0)
            sprite.setColor(sf::Color(255, 255, 255, 255)); 
        else
            sprite.setColor(sf::Color(255, 255, 255, 50)); 
    }
}

void PowerUp::draw(sf::RenderWindow& window) {
    if (active)
        window.draw(sprite);
}

bool PowerUp::isActive() const {
    return active;
}

void PowerUp::deactivate() {
    active = false;
}

sf::FloatRect PowerUp::getBounds() const {
    return sprite.getGlobalBounds();
}
