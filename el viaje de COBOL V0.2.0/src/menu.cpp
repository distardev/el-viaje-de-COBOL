#include "menu.hpp"
#include "resources.hpp"
#include <iostream>

GameResources menuResources;

Menu::Menu(AudioManager& audioManager) 
: audio(audioManager)
 {
    menuResources.load();

    spriteBgMenu.setTexture(menuResources.textureMenuBackground);
    spriteBgMenu.setPosition(0, 0);

    spriteTitleMenu.setTexture(menuResources.textureMenuTitle);
    spriteTitleMenu.setPosition(105, 30);

    spriteMuteMusic.setTexture(menuResources.textureUnmuteMusic);
    spriteMuteMusic.setPosition(70, 60);

    spriteMuteSFX.setTexture(menuResources.textureUnmuteSFX);
    spriteMuteSFX.setPosition(165, 60);

    spriteContinue.setTexture(menuResources.textureContinue);
    spriteContinue.setPosition(60, 120);

    spriteExit.setTexture(menuResources.textureExit);
    spriteExit.setPosition(145, 120);

    cursorSprite.setTexture(menuResources.textureCursor);
    cursorSprite.setScale(1.5f, 1.5f);

    volumeMaster = audio.getMasterVolume();
    volumeMusic = savedMusicVolume = audio.getMusicVolume();
    volumeEffects = savedSFXVolume = audio.getSFXVolume();
}

void Menu::updateCursorPosition(sf::RenderWindow& window) {
    if (!visible) return;

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos); 
    cursorSprite.setPosition(worldPos);
}

void Menu::draw(sf::RenderWindow& window) {
    if (!visible) return;

    window.draw(spriteBgMenu);
    window.draw(spriteTitleMenu);
    window.draw(spriteMuteMusic);
    window.draw(spriteMuteSFX);
    window.draw(spriteContinue);
    window.draw(spriteExit);
    window.draw(cursorSprite);
}

void Menu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!visible) return;

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (spriteContinue.getGlobalBounds().contains(mousePos)) {
            if (onContinue) onContinue();
        } else if (spriteExit.getGlobalBounds().contains(mousePos)) {
            if (onExit) onExit();
        } else if (spriteMuteMusic.getGlobalBounds().contains(mousePos)) {
            musicMuted = !musicMuted;
            if (musicMuted) {
                savedMusicVolume = volumeMusic;
                volumeMusic = 0.f;
                spriteMuteMusic.setTexture(menuResources.textureMuteMusic);
            } else {
                volumeMusic = savedMusicVolume;
                spriteMuteMusic.setTexture(menuResources.textureUnmuteMusic);
            }
        } else if (spriteMuteSFX.getGlobalBounds().contains(mousePos)) {
            sfxMuted = !sfxMuted;
            if (sfxMuted) {
                savedSFXVolume = volumeEffects;
                volumeEffects = 0.f;
                spriteMuteSFX.setTexture(menuResources.textureMuteSFX);
            } else {
                volumeEffects = savedSFXVolume;
                spriteMuteSFX.setTexture(menuResources.textureUnmuteSFX);
            }
        }

        audio.setVolume(volumeMaster, volumeMusic, volumeEffects);
    }
}


void Menu::setVisible(bool v) {
    visible = v;
}

bool Menu::isVisible() const {
    return visible;
}



float Menu::getMasterVolume() const {
    return volumeMaster;
}

float Menu::getMusicVolume() const {
    return volumeMusic;
}

float Menu::getSFXVolume() const {
    return volumeEffects;
}

