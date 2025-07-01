#pragma once
#include <audio_manager.hpp>
#include <SFML/Graphics.hpp>
#include <functional>

class Menu {
public:
    Menu(AudioManager& audioManager); 

    sf::Font font;

    void updateCursorPosition(sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    
    void setVisible(bool visible);
    bool isVisible() const;
    
    std::function<void()> onContinue;
    std::function<void()> onExit;

    float getMasterVolume() const;
    float getMusicVolume() const;
    float getSFXVolume() const;


private:
    sf::Text muteMusicButton, muteSFXButton;
    bool musicMuted = false;
    bool sfxMuted = false;
    float savedMusicVolume = 100.f;
    float savedSFXVolume = 100.f;

    sf::Sprite spriteTitleMenu;
    sf::Sprite spriteBgMenu;
    sf::Sprite spriteMuteMusic;
    sf::Sprite spriteMuteSFX;
    sf::Sprite spriteContinue;
    sf::Sprite spriteExit;
    sf::Sprite cursorSprite;

    bool visible = false;

    AudioManager& audio; 

    float volumeMaster = 100.f;
    float volumeMusic = 100.f;
    float volumeEffects = 100.f;
};
