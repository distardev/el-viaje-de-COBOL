#pragma once
#include <SFML/Graphics.hpp>    
#include <vector>
#include "bullet.hpp"
#include "audio_manager.hpp"

class Player {
private:
    sf::Texture texture;
    sf::Sprite sprite;

    sf::Clock animationClock;
    int currentFrame = 0;
    int row = 1;
    float animationSpeed = 0.2f;

    float speed = 60.f;
    sf::Vector2f aimDirection = {1.f, 0.f};
    sf::Vector2f direction = {1.f, 0.f};
    bool isMoving = false;
    const float limit = 18.f;
    sf::Vector2f pos;

    float bulletCooldown = 0.35f;
    float bulletTimer = 0.f;
    int fireLevel = 1;
    std::vector<Bullet> bullets;
    const sf::Texture* blueBulletTexture = nullptr;
    const sf::Texture* firedBulletTexture = nullptr;

    sf::Sprite crosshair;
    sf::Texture crosshairTexture;
    sf::Vector2f aimDirectionJoystick;
    sf::Vector2f aimDirectionMouse;

    int lives = 3;
    bool invulnerable = false;
    float invulnerableTime = 0.0f;
    const float invulnerableDuration = 3.0f;

    AudioManager* audio = nullptr;

public:
    Player(AudioManager* audioManager, const sf::Texture* blueTex, const sf::Texture* firedTex);
    void update(float dt, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void shoot(float dt);
    void setFireLevel(int level);
    sf::Vector2f getPosition() const;

    sf::FloatRect getHitbox() const {
        return sprite.getGlobalBounds();
    }

    sf::Vector2f getInputDirection();

    sf::FloatRect getBounds() const;
    std::vector<Bullet>& getBullets();

    void setBulletTextures(const sf::Texture* blueTex, const sf::Texture* firedTex) {
        blueBulletTexture = blueTex;
        firedBulletTexture = firedTex;
    }

    int getLives() const { return lives; }
    int getFireLevel() const { return fireLevel; }
    void applyDamage(int dmg, const sf::Vector2f& sourcePos);

    sf::Clock damageClock;
    sf::Vector2f knockbackVelocity;
    float getInvulnerableTime() const { return invulnerableTime; }
    bool isInvulnerable() const { return invulnerable; }

    void updateAimDirectionAndCrosshair(sf::RenderWindow& window);
};
