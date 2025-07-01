#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

enum class BulletType { BlueBullet, FiredBullet };
class Bullet {
public:
    Bullet(sf::Vector2f pos, sf::Vector2f dir, float speed, int level, BulletType type, const sf::Texture& texture);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isOffScreen() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    int getDamage() const;

private:
    sf::Sprite sprite;
    BulletType type;
    sf::Vector2f position;
    sf::Vector2f direction;
    float speed;
    int damage = 1;
};
