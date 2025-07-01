#include "dynamic_background.hpp"
#include <iostream>

DynamicBackground::DynamicBackground() {
    if (!shader.loadFromFile("assets/sprites/backgrounds/background.frag", sf::Shader::Fragment)) {
        std::cerr << "Error al cargar el shader del fondo dinámico.\n";
    }

}

bool DynamicBackground::load() {
    if (!texture.loadFromFile("assets/sprites/backgrounds/background.png")) {
        std::cerr << "No se pudo cargar background.png\n";
        return false;
    }
    sprite.setTexture(texture);
    sprite.setPosition(0, 0);
    return true;
}

void DynamicBackground::update(float time, const sf::RenderWindow& window) {
    shader.setUniform("time", time);
    shader.setUniform("texture", sf::Shader::CurrentTexture);
    shader.setUniform("resolution", sf::Vector2f(window.getSize()));
}

void DynamicBackground::draw(sf::RenderWindow& window) {
    window.draw(sprite, &shader);
}
