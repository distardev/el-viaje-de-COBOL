#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class DynamicBackground {
public:
    DynamicBackground();
    bool load();
    void update(float time, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Shader shader;
};
