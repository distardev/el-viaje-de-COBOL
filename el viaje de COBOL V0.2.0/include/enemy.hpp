#pragma once
#include <SFML/Graphics.hpp>     
#include "audio_manager.hpp"
#include <bullet.hpp>

enum class EnemyType { Skeletons, FiredSkeletons, PitcherSkeletons };

class Enemy {
private:
    sf::Sprite sprite;
    const sf::Texture* texture;

    sf::Vector2f position;
    EnemyType type;

    int currentFrame = 0;
    int row = 0;
    float animationSpeed = 0.2f;
    sf::Clock animationClock;

    int health;
    float speed;
    bool canShoot;
    int damage;

    sf::Clock shootClock;
    float shootCooldown = 2.0f;
    const sf::Texture* bulletTexture = nullptr;

    bool hit = false;
    float hitTimer = 0.f;
    static constexpr float hitDuration = 0.15f;

    sf::Vector2f knockbackVelocity{0.f, 0.f};

    AudioManager* audio = nullptr; 
    bool deathSoundPlayed = false; 
public:
    explicit Enemy(EnemyType type, int level, sf::Vector2f spawnPos, const sf::Texture& sharedTexture, AudioManager* audio, const sf::Texture* bulletTex);

    void update(const sf::Vector2f& playerPos, int level, float dt);

    void draw(sf::RenderWindow& window);
    bool isAlive() const;
    int getDamage() const;
    void applyDamage(int amount, const sf::Vector2f& sourcePos);

    sf::Vector2f getPosition() const { return position; }

    sf::FloatRect getHitbox() const {
        return sprite.getGlobalBounds();
    }
    sf::FloatRect getBounds() const;

    void move(const sf::Vector2f& offset);

    std::vector<Bullet> shootFireballs(const sf::Vector2f& playerPos, float dt);
};
