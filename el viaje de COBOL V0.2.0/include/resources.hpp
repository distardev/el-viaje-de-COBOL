#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

struct GameResources {
    std::vector<sf::Texture> grasswallTexture;

    sf::Texture blueBulletTexture;
    sf::Texture firedBulletTexture;

    sf::Texture skeletonTexture;
    sf::Texture firedSkeletonTexture;
    sf::Texture pitcherSkeletonTexture;

    sf::Texture coinTexture;

    // Sprites de Menu
    sf::Texture textureMenuBackground;
    sf::Texture textureMenuTitle;
    sf::Texture textureMuteMusic, textureUnmuteMusic;
    sf::Texture textureMuteSFX, textureUnmuteSFX;
    sf::Texture textureContinue, textureExit;
    sf::Texture textureCursor;

    void load();
};

#endif
