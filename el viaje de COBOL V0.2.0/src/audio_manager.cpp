#include "audio_manager.hpp"
#include <iostream>

bool AudioManager::load(){
    auto levelMusic = std::make_unique<sf::Music>();
    if (!levelMusic->openFromFile("assets/sounds/music/game_level_music.wav")) return false;
    musics["level"] = std::move(levelMusic);

    std::map<std::string, std::string> sfxFiles = {
        {"coin", "assets/sounds/sfx/coin_pickup.wav"},
        {"damage", "assets/sounds/sfx/player_damage.wav"},
        {"shot1", "assets/sounds/sfx/shot1.wav"},
        {"shot2", "assets/sounds/sfx/shot2.wav"},
        {"shot3", "assets/sounds/sfx/shot3.wav"},
        {"skeleton_die", "assets/sounds/sfx/skeleton_die.wav"},
        {"fired_skeleton_die", "assets/sounds/sfx/fired_skeleton_die.wav"},
        {"upgrade", "assets/sounds/sfx/upgrade.wav"},
    };

    for (const auto& [name, path] : sfxFiles){
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(path)){    
            std::cerr << "No se pudo cargar: " << path << "\n";
            continue;
        }
        sfxBuffers[name] = buffer;
        sf::Sound sound;
        sound.setBuffer(sfxBuffers[name]);
        sfx[name] = sound;

    }

    return true;

}

void AudioManager::playMusic(const std::string& name) {
    stopMusic(); 
    if (musics.count(name)) {
        musics[name]->setLoop(true);
        musics[name]->setVolume(musicVolume * (masterVolume / 100.f));
        musics[name]->play();
    }
}

void AudioManager::stopMusic() {
    for (auto& [_, music] : musics) {
        music->stop();
    }
}

void AudioManager::playSFX(const std::string& name) {
    if (sfxBuffers.find(name) == sfxBuffers.end()) {
        std::cerr << "SFX no encontrado: " << name << "\n";
        return;
    }

    auto sound = std::make_unique<sf::Sound>();
    sound->setBuffer(sfxBuffers[name]);
    sound->setVolume(sfxVolume * (masterVolume / 100.f));
    sound->play();

    activeSounds.push_back(std::move(sound));
}

void AudioManager::setVolume(float master, float music, float sfxVol) {
    masterVolume = master;
    musicVolume = music;
    sfxVolume = sfxVol;

    for (auto& [_, music] : musics)
        music->setVolume(musicVolume * (masterVolume / 100.f));

    for (auto& sound : activeSounds)
        sound->setVolume(sfxVolume * (masterVolume / 100.f));
}

float AudioManager::getMasterVolume() const { return masterVolume; }
float AudioManager::getMusicVolume() const { return musicVolume; }
float AudioManager::getSFXVolume() const { return sfxVolume; }

void AudioManager::update() {
    activeSounds.erase(
        std::remove_if(
            activeSounds.begin(), activeSounds.end(),
            [](const std::unique_ptr<sf::Sound>& s) {
                return s->getStatus() == sf::Sound::Stopped;
            }),
        activeSounds.end()
    );
}