#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory> 

class AudioManager {
public:
    bool load();
    void update();
    
    void playMusic(const std::string& name);
    void stopMusic();
    void playSFX(const std::string& name);

    void setVolume(float master, float music, float sfx);
    float getMasterVolume() const;
    float getMusicVolume() const;
    float getSFXVolume() const;

private:
    std::map<std::string, std::unique_ptr<sf::Music>> musics;
    std::map<std::string, sf::SoundBuffer> sfxBuffers;
    std::vector<std::unique_ptr<sf::Sound>> activeSounds;
    std::map<std::string, sf::Sound> sfx;

    float masterVolume = 100.f;
    float musicVolume = 100.f;
    float sfxVolume = 100.f;
};