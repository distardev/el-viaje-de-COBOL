#include "money.hpp"

Money::Money(sf::Vector2f pos, const sf::Texture& texture, AudioManager* audioManager) 
 : position(pos), audio(audioManager)
 {
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    sprite.setPosition(position);
}

void Money::update(float dt) {
    if (collected) return;
    
    animationTimer += dt;
    if (animationTimer >= 0.35f) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % 3;
        sprite.setTextureRect(sf::IntRect(currentFrame * 16, 0, 16, 16));
    }

    lifetime += dt;
    if (lifetime >= maxLifetime) {
        active = false; 
        return;
    }

    if (lifetime >= blinkStart) {
        float blinkPeriod = 0.4f;
        int blinkPhase = static_cast<int>((lifetime - blinkStart) / blinkPeriod) % 2;
        if (blinkPhase == 0)
            sprite.setColor(sf::Color(255, 255, 255, 255)); 
        else
            sprite.setColor(sf::Color(255, 255, 255, 50)); 
    }
}

void Money::draw(sf::RenderWindow& window) {
    if (!collected)
        window.draw(sprite);
}

bool Money::checkCollision(const sf::FloatRect& playerBounds) {
    if (!collected && sprite.getGlobalBounds().intersects(playerBounds)) {
        collected = true;
        if (audio) audio->playSFX("coin");
        return true;
    }
    return false;
}

bool Money::isCollected() const {
    return collected;
}

bool Money::isActive() const {
    return active;
}
