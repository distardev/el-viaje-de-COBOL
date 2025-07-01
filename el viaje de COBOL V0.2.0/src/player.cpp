#include "player.hpp"
#include "audio_manager.hpp"
#include <cmath>
#include <iostream>

Player::Player(AudioManager* audioManager, const sf::Texture* blueTex, const sf::Texture* firedTex)  
: audio(audioManager), blueBulletTexture(blueTex), firedBulletTexture(firedTex)
{
    if (!texture.loadFromFile("assets/sprites/player/player_spritesheet.png")) {
        std::cerr << "Error al cargar el sprite del jugador\n";
    }
    sprite.setTexture(texture);
    currentFrame = 0;
    row = 1;
    sprite.setTextureRect(sf::IntRect(currentFrame * 16, row * 16, 16, 16));
    sprite.setOrigin(8, 8);

    pos = {160.f, 88.f};
    sprite.setPosition(pos);

    if (!crosshairTexture.loadFromFile("assets/sprites/ui/crosshair.png")) {
        std::cerr << "Error al cargar la textura de la mira\n";
    }
    crosshair.setTexture(crosshairTexture);
    crosshair.setOrigin(crosshairTexture.getSize().x / 2.f, crosshairTexture.getSize().y / 2.f);
}

sf::Vector2f Player::getInputDirection() {
    sf::Vector2f dir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x += 1.f;

    if (sf::Joystick::isConnected(0)) {
        float dpadX = sf::Joystick::getAxisPosition(0, sf::Joystick::PovX);
        float dpadY = sf::Joystick::getAxisPosition(0, sf::Joystick::PovY);

        if (dpadX < -50) dir.x -= 1.f;
        if (dpadX > 50)  dir.x += 1.f;

        if (dpadY < -50) dir.y += 1.f;
        if (dpadY > 50)  dir.y -= 1.f;
    }

    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len != 0.f)
        dir /= len;

    return dir;
}

void Player::update(float dt, sf::RenderWindow& window) {
    const float limit = 18.f;
    const float minX = limit;
    const float maxX = 268.f - limit;
    const float minY = limit;
    const float maxY = 171.f - limit;

    if (pos.x < minX) pos.x = minX;
    if (pos.x > maxX) pos.x = maxX;
    if (pos.y < minY) pos.y = minY;
    if (pos.y > maxY) pos.y = maxY;

    sf::Vector2f movement = getInputDirection();
    isMoving = (movement.x != 0.f || movement.y != 0.f);

    if (isMoving) {
        pos += direction * speed * dt;
        if (animationClock.getElapsedTime().asSeconds() > animationSpeed) {
            currentFrame = 1 + (currentFrame % 2);
            animationClock.restart();
        }

        if (movement.y < 0)       row = 0;
        else if (movement.y > 0)  row = 1;
        if (movement.x < 0)       row = 2;
        else if (movement.x > 0)  row = 3;

        if (std::abs(movement.x) > 0.5f && std::abs(movement.y) > 0.5f) {
            if (movement.x > 0 && movement.y < 0)  row = 4;
            if (movement.x > 0 && movement.y > 0)  row = 5;
            if (movement.x < 0 && movement.y < 0)  row = 6;
            if (movement.x < 0 && movement.y > 0)  row = 7;
        }

        sprite.setTextureRect(sf::IntRect(currentFrame * 16, row * 16, 16, 16));

        direction    = movement;
        aimDirection = movement;
    }
    else {
        sprite.setTextureRect(sf::IntRect(0, row * 16, 16, 16));
        switch (row) {
            case 0:  aimDirection = {  0.f, -1.f }; break;
            case 1:  aimDirection = {  0.f,  1.f }; break;
            case 2:  aimDirection = { -1.f,  0.f }; break;
            case 3:  aimDirection = {  1.f,  0.f }; break;
            case 4:  aimDirection = {  0.707f, -0.707f }; break;
            case 5:  aimDirection = {  0.707f,  0.707f }; break;
            case 6:  aimDirection = { -0.707f, -0.707f }; break;
            case 7:  aimDirection = { -0.707f,  0.707f }; break;
        }
        direction = { 0.f, 0.f };
    }


    if (knockbackVelocity.x != 0.f || knockbackVelocity.y != 0.f) {
        pos += knockbackVelocity * dt;
        knockbackVelocity *= 0.85f;
        if (std::abs(knockbackVelocity.x) < 1.f && std::abs(knockbackVelocity.y) < 1.f) {
            knockbackVelocity = { 0.f, 0.f };
        }
    }

    const float margin = 8.f;
    pos.x = std::clamp(pos.x, margin, 320.f - margin);
    pos.y = std::clamp(pos.y, margin, 176.f - margin);

    updateAimDirectionAndCrosshair(window);

    if (invulnerable) {
        invulnerableTime += dt;
        if (invulnerableTime >= 1.5f) {
            sprite.setColor(sf::Color::White);
            invulnerable = false;
        } else {
            float cycle = std::fmod(invulnerableTime, 0.5f);
            sf::Uint8 alpha = (cycle < 0.125f) ? 255u : 100u;
            sprite.setColor(sf::Color(255, 255, 255, alpha));
        }
    }

    bulletTimer += dt;
    bool shootPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
    sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (sf::Joystick::isConnected(0)) {
    shootPressed = shootPressed || 
    sf::Joystick::isButtonPressed(0, 0) ||
    sf::Joystick::isButtonPressed(0, 3) ||
    sf::Joystick::isButtonPressed(0, 5) ||
    sf::Joystick::isButtonPressed(0, 7);
  }

    if (shootPressed && bulletTimer >= bulletCooldown) {
    shoot(dt);
    bulletTimer = 0.f;
    }
    for (auto it = bullets.begin(); it != bullets.end();) {
        it->update(dt);
        if (it->isOffScreen()) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }

    sprite.setPosition(pos);
}

void Player::updateAimDirectionAndCrosshair(sf::RenderWindow& window) {
    const float maxCrosshairDistance = 60.f;

    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePixelPos);

    sf::Vector2f toMouse = mousePosWorld - pos;

    float distance = std::sqrt(toMouse.x * toMouse.x + toMouse.y * toMouse.y);

    if (distance > maxCrosshairDistance) {
        toMouse /= distance;
        toMouse *= maxCrosshairDistance;
    }

    crosshair.setPosition(pos + toMouse);

    if (distance != 0.f)
        aimDirectionMouse = toMouse / std::sqrt(toMouse.x * toMouse.x + toMouse.y * toMouse.y);
    else
        aimDirectionMouse = { 1.f, 0.f };

    sf::Vector2f inputAimDir = aimDirectionMouse;

    if (sf::Joystick::isConnected(0)) {
        float axisX = sf::Joystick::getAxisPosition(0, sf::Joystick::U);
        float axisY = sf::Joystick::getAxisPosition(0, sf::Joystick::V);

        if (std::abs(axisX) > 15.f || std::abs(axisY) > 15.f) {
            aimDirectionJoystick = { axisX, axisY };
            float lenJoy = std::sqrt(axisX * axisX + axisY * axisY);
            if (lenJoy != 0.f)
                aimDirectionJoystick /= lenJoy;

            inputAimDir = aimDirectionJoystick;
        }
    }

    aimDirection = inputAimDir;

    float angle = std::atan2(aimDirection.y, aimDirection.x) * 180.f / 3.14159265f;

    if (angle > -22.5f && angle <= 22.5f)         row = 3;
    else if (angle > 22.5f && angle <= 67.5f)     row = 5;
    else if (angle > 67.5f && angle <= 112.5f)    row = 1;
    else if (angle > 112.5f && angle <= 157.5f)   row = 7;
    else if (angle > 157.5f || angle <= -157.5f)  row = 2;
    else if (angle > -157.5f && angle <= -112.5f) row = 6;
    else if (angle > -112.5f && angle <= -67.5f)  row = 0;
    else if (angle > -67.5f && angle <= -22.5f)   row = 4;

    sprite.setTextureRect(sf::IntRect((isMoving ? currentFrame : 0) * 16, row * 16, 16, 16));
}



    sf::Vector2f rotateVector(const sf::Vector2f& v, float degrees) {
    float rad = degrees * 3.14159265f / 180.f;
    float cs = std::cos(rad);
    float sn = std::sin(rad);
    return sf::Vector2f(v.x * cs - v.y * sn, v.x * sn + v.y * cs);
}

    void Player::shoot(float dt) {
        switch (fireLevel) {
            case 1:
                if (blueBulletTexture) {
                    bullets.emplace_back(pos, aimDirection, 5.f, fireLevel, BulletType::BlueBullet, *blueBulletTexture);
                }                
                if (audio) audio->playSFX("shot1");
                break;
            case 2: {
                if (blueBulletTexture) {
                    bullets.emplace_back(pos, rotateVector(aimDirection, -15.f), 5.f, fireLevel, BulletType::BlueBullet, *blueBulletTexture);
                    bullets.emplace_back(pos, aimDirection, 5.f, fireLevel, BulletType::BlueBullet, *blueBulletTexture);
                    bullets.emplace_back(pos, rotateVector(aimDirection, 15.f), 5.f, fireLevel, BulletType::BlueBullet, *blueBulletTexture);
                }
                if (audio) audio->playSFX("shot1");
                break;
            }
            case 3: {
            if (blueBulletTexture) {
                Bullet laser(pos, aimDirection, 10.f, fireLevel, BulletType::BlueBullet, *blueBulletTexture);
                laser.update(dt);
                bullets.push_back(laser);
            }
                if (audio) audio->playSFX("shot1");
                break;
            }
        }
    }

void Player::setFireLevel(int level) {
    fireLevel = std::clamp(level, 1, 3);
    switch (fireLevel) {
        case 1: bulletCooldown = 0.35f; break;
        case 2: bulletCooldown = 0.60f; break;
        case 3: bulletCooldown = 0.20f; break;
    }
}

sf::Vector2f Player::getPosition() const {
    return pos;
}

sf::FloatRect Player::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds;
}

std::vector<Bullet>& Player::getBullets() {
    return bullets;
}

void Player::applyDamage(int dmg, const sf::Vector2f& sourcePos) {
    if (!invulnerable) {
        lives -= dmg;
        if (lives < 0) lives = 0;

        if (audio) audio->playSFX("damage");

        invulnerable = true;
        invulnerableTime = 0.f;
        damageClock.restart();

        sprite.setColor(sf::Color(255, 255, 255, 180));

        sf::Vector2f direction = pos - sourcePos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0)
            direction /= length;

        knockbackVelocity = direction * 150.f; 

    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    for (auto& b : bullets)                        
        b.draw(window);
    window.draw(crosshair);
}