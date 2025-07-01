#include "bullet.hpp"
#include <iostream>
#include <cmath>

// Constructor de bala: recibe posición, dirección, velocidad y nivel visual
Bullet::Bullet(sf::Vector2f pos, sf::Vector2f dir, float speed, int level, BulletType bulletType, const sf::Texture& texture)
    : position(pos), speed(speed), type(bulletType)
{    

    // Normalizar dirección (evita que la bala viaje más rápido en diagonal)
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length != 0.f)
        direction = dir / length;
    else
        direction = sf::Vector2f(0.f, 0.f);

    // Configuración del sprite
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect((level - 1) * 16, 0, 16, 16));    // Frame según nivel
    sprite.setOrigin(8, 8);                                             // Centro de rotación/dibujo
    sprite.setPosition(position);

    // Rotar sprite para que apunte hacia donde va
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle);
}

// Actualiza posición de la bala según dirección y velocidad
void Bullet::update(float dt) {
    position += direction * speed;
    sprite.setPosition(position);
}

// Dibuja la bala en pantalla
void Bullet::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Retorna true si la bala está fuera de los límites de la pantalla
bool Bullet::isOffScreen() const {
    return position.y < 0 || position.y > 176 || position.x < 0 || position.x > 320;
}

// Retorna el área rectangular del sprite para detectar colisiones
sf::FloatRect Bullet::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Bullet::getPosition() const {
    return position;
}

int Bullet::getDamage() const {
    return damage;
}
