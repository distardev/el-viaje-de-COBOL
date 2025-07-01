#include "enemy.hpp"
#include <cmath>

Enemy::Enemy(EnemyType enemyType, int level, sf::Vector2f spawnPos, const sf::Texture& sharedTexture, AudioManager* audioManager, const sf::Texture* bulletTex)
    : type(enemyType), position(spawnPos), texture(&sharedTexture), audio(audioManager), bulletTexture(bulletTex)
    {
        
    sprite.setTexture(*texture);

    this->bulletTexture = bulletTex;
    shootClock.restart();

    if (type == EnemyType::Skeletons) {
        speed = 0.3f + 0.25f;
        canShoot = false;
        row = 0;
        health = 3;
        damage = 1;
    } else if (type == EnemyType::FiredSkeletons) {
        speed = 0.8f + 0.25f;
        canShoot = false;
        row = 0;
        health = 4;
        damage = 2;
    } else if (type == EnemyType::PitcherSkeletons) {
        speed = 0.15f + 0.25f;
        canShoot = true;
        row = 0;
        health = 4;
        damage = 2;
        shootCooldown = 2.0f;
    }
    
    sprite.setOrigin(8, 8);
    sprite.setPosition(position);
    sprite.setTextureRect(sf::IntRect(currentFrame * 16, row * 16, 16, 16));
}

void Enemy::update(const sf::Vector2f& playerPos, int level, float dt) {
    sf::Vector2f direction = playerPos - position;

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0.f) {
        direction /= length;
    }

    float distanceToPlayer = length;

    if (type == EnemyType::PitcherSkeletons) {
        if (distanceToPlayer < 60.f) {
            // Muy cerca → se aleja
            position -= direction * speed;
        } else if (distanceToPlayer > 100.f) {
            // Muy lejos → se acerca
            position += direction * speed;
        }
        // Si está en rango intermedio (ideal), no se mueve
    } else {
        // Comportamiento normal para otros enemigos
        position += direction * speed;
    }

    sprite.setPosition(position);

    if (direction.y < -0.5f) row = 0;
    else if (direction.y > 0.5f) row = 1;
    if (direction.x < -0.5f) row = 2;
    else if (direction.x > 0.5f) row = 3;

    if (std::abs(direction.x) > 0.5f && std::abs(direction.y) > 0.5f) {
        if (direction.x > 0 && direction.y < 0) row = 4;
        if (direction.x > 0 && direction.y > 0) row = 5;
        if (direction.x < 0 && direction.y < 0) row = 6;
        if (direction.x < 0 && direction.y > 0) row = 7;
    }

    if (animationClock.getElapsedTime().asSeconds() > animationSpeed) {
        currentFrame = (currentFrame + 1) % 3;
        animationClock.restart();
    }

    sprite.setTextureRect(sf::IntRect(currentFrame * 16, row * 16, 16, 16));


    if (hit) {
        hitTimer += dt;
        if (hitTimer < hitDuration) {
            float flickerCycle = std::fmod(hitTimer, 0.1f);
            sf::Uint8 alpha = (flickerCycle < 0.05f) ? 100u : 255u;
            sprite.setColor(sf::Color(255, 255, 255, alpha));
        } else {
            hit = false;
            hitTimer = 0.f;
            sprite.setColor(sf::Color::White);
        }
    }

    if (knockbackVelocity.x != 0.f || knockbackVelocity.y != 0.f) {
        sprite.move(knockbackVelocity * dt);
        knockbackVelocity *= 0.85f;
        if (std::abs(knockbackVelocity.x) < 1.f && std::abs(knockbackVelocity.y) < 1.f) {
            knockbackVelocity = {0.f, 0.f};
        }
        position = sprite.getPosition();
    }
}

std::vector<Bullet> Enemy::shootFireballs(const sf::Vector2f& playerPos, float dt) {
    std::vector<Bullet> bullets;

    if (canShoot && shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
        shootClock.restart();

        // Dirección al jugador
        sf::Vector2f direction = sprite.getPosition();  // Posición de enemigo
        sf::Vector2f dir = playerPos - direction;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len != 0.f)
            dir /= len;

        // Crear bala
        Bullet bullet(position, dir, 2.f, 1, BulletType::FiredBullet, *bulletTexture);
        bullets.push_back(bullet);

        if (audio) audio->playSFX("shot2");
    }

    return bullets;
}


void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Enemy::isAlive() const {
    return this->health > 0;
}

int Enemy::getDamage() const {
    return damage;               
}

void Enemy::applyDamage(int amount, const sf::Vector2f& sourcePos) {
    health -= amount;
    if (health < 0) health = 0;

    if (health == 0 && !deathSoundPlayed && audio) {
        audio->playSFX("skeleton_die"); 
        deathSoundPlayed = true;
    }

    hit = true;
    hitTimer = 0.f;

    sf::Vector2f directionKB = sprite.getPosition() - sourcePos;
    float len = std::sqrt(directionKB.x*directionKB.x + directionKB.y*directionKB.y);
    if (len != 0) directionKB /= len;
    knockbackVelocity = directionKB * 50.f;
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

void Enemy::move(const sf::Vector2f& offset) {
    position += offset;
    sprite.setPosition(position);
}