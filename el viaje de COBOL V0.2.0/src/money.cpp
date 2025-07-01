#include "money.hpp"

Money::Money(sf::Vector2f pos, const sf::Texture& texture, AudioManager* audioManager) 
 : position(pos), audio(audioManager)
 {
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    sprite.setPosition(position);
}

void Money::update(float deltaTime) {
    if (collected) return;

    animationTimer += deltaTime;
    if (animationTimer >= 0.15f) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % 3;
        sprite.setTextureRect(sf::IntRect(currentFrame * 16, 0, 16, 16));
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
