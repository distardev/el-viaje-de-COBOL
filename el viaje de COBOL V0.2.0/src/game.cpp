#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.hpp"
#include "enemy.hpp"
#include "ui.hpp"
#include "money.hpp"
#include "menu.hpp"
#include "audio_manager.hpp"
#include "resources.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "dynamic_background.hpp"

// ----------------------------------------------------------------------
// CONSTANTES
// ----------------------------------------------------------------------

int runGame();
int level = 1;

// Límites del mapa
const float limit = 18.f;
const float minX = limit;
const float maxX = 319.f - limit;
const float minY = limit;
const float maxY = 171.f - limit;

// Definimos los sprites
sf::Sprite backgroundSprite;
sf::Sprite grasswallSprite;
std::vector<Enemy> enemies;
std::vector<Bullet> enemyBullets;

DynamicBackground dynamicBackground;
sf::Shader shader;

GameResources resources;
AudioManager audio;

// Esta función evita que los enemigos atraviesen al jugador
void repelEnemiesFromPlayer(std::vector<Enemy>& enemies, const Player& player, float minDistance) {
    sf::Vector2f playerPos = player.getPosition();

    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;

        sf::Vector2f enemyPos = enemy.getPosition();
        sf::Vector2f delta = enemyPos - playerPos;
        float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (distance < minDistance && distance > 0.0f) {
            sf::Vector2f push = (delta / distance) * (minDistance - distance);
            enemy.move(push);
        }
    }
}

// Cambio dinámico de fondo y texto
void setLevel(int newLevel, UI& ui) {
    level = std::clamp(newLevel, 1, 4);
    grasswallSprite.setTexture(resources.grasswallTexture[level - 1]);
    ui.setLevel(level);
}

// ----------------------------------------------------------------------
// FUNCIÓN PRINCIPAL
// ----------------------------------------------------------------------


int runGame(){
    resources.load();

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "el viaje de COBOL", sf::Style::Resize | sf::Style::Close);

        // Ventana escalada (3x de 320x176)
    // sf::RenderWindow window(sf::VideoMode(960, 528), "el viaje de COBOL", sf::Style::Close);
    window.setFramerateLimit(60);

    // Escalar la vista lógica (permite trabajar como si fuera 320x176)
    sf::View view(sf::FloatRect(0, 0, 320, 176));
    window.setView(view);

    // -----------------------------------------
    // VARIABLES DEL JUEGO
    // -----------------------------------------
    
    UI ui;
    ui.loadTextures();
    ui.setLevel(1);
    ui.setProgress(0, 15);
    ui.setLives(3);
    ui.setMoney(0);

    setLevel(1, ui);

    dynamicBackground.load();    

    grasswallSprite.setTexture(resources.grasswallTexture[level - 1]);
    grasswallSprite.setPosition(0.f, 0.f); 


    Player player(&audio, &resources.blueBulletTexture, &resources.firedBulletTexture);

    Menu menu(audio);

    menu.onContinue = [&]() {
        menu.setVisible(false);
    };

    menu.onExit = [&]() {
        window.close();
    };

    sf::Clock clock;

    struct SpawnRange {
        float x;
        float minY;
        float maxY;
    };

    sf::Clock shaderClock;
    
    bool gameOver = false;
    sf::Clock gameOverClock;
    
    std::vector<Money> coins;

    float masterVolume = 100.f;
    float musicVolume = 100.f;
    float sfxVolume = 100.f;

    audio.load();
    audio.playMusic("level"); // iniciar música de nivel

    // -----------------------------------------
    // SPAWN POSITION DE ENEMIGOS
    // -----------------------------------------

    std::vector<std::vector<SpawnRange>> spawnPositions = {
        { {-15, 70, 100} },                                                            // nivel 1
        { {-15, 70, 100}, {335, 70, 100} },                                           // nivel 2
        { {-15, 70, 100}, {335, 70, 100}, {160, -15, -15} },                         // nivel 3
        { {-15, 70, 100}, {335, 70, 100}, {160, 185, 185}, {160, -15, -15} }        // nivel 4
    };
    srand(static_cast<unsigned>(time(nullptr)));

    // -----------------------------------------
    // VARIABLES PARA MANEJAR OLAS (WAVES) 
    // -----------------------------------------

    int currentWave = 1;           // Ola 1 => spawnear 1 enemigo
    int totalKilled = 0;           // Contador total de enemigos eliminados
    int totalToKill = 15;          // Después de 15 enemigos, pasamos a nivel 2

    // Función lambda para spawnear "n" enemigos usando currentWave
    auto spawnWave = [&](int waveCount) {
        const auto& levelSpawns = spawnPositions[level - 1];
    
        for (int i = 0; i < waveCount; ++i) {
            const auto& spawn = levelSpawns[i % levelSpawns.size()];
    
            float randomY = spawn.minY + static_cast<float>(rand()) / RAND_MAX * (spawn.maxY - spawn.minY);
            sf::Vector2f spawnPos(spawn.x, randomY);
    
            EnemyType enemyType = EnemyType::Skeletons;

            
            if (level >= 3) {
                int rng = rand() % 100;
                if (rng < 20) {
                    enemyType = EnemyType::PitcherSkeletons; 
                } else if (rng < 50) {
                    enemyType = EnemyType::FiredSkeletons; 
                }
            } else if (level >= 2) {
                if ((rand() % 100) < 10) {
                    enemyType = EnemyType::FiredSkeletons;  
                }
            }

            switch (enemyType) {
                case EnemyType::Skeletons:
                    enemies.emplace_back(EnemyType::Skeletons, level, spawnPos, resources.skeletonTexture, &audio, &resources.firedBulletTexture);
                    break;
                case EnemyType::FiredSkeletons:
                    enemies.emplace_back(EnemyType::FiredSkeletons, level, spawnPos, resources.firedSkeletonTexture, &audio, &resources.firedBulletTexture);
                    break;
                case EnemyType::PitcherSkeletons:
                    enemies.emplace_back(EnemyType::PitcherSkeletons, level, spawnPos, resources.pitcherSkeletonTexture, &audio, &resources.firedBulletTexture);
                    break;
            }
        }
    };
    
    // Spawneamos la primera ola (1 enemigo) antes de entrar al bucle
    spawnWave(currentWave);

    // ----------------------------------------------------------------------
    // GAME LOOP
    // ----------------------------------------------------------------------  

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            // Verificamos si menú está abierto al presional el botón Escape
            if (!menu.isVisible() && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                menu.setVisible(true);
            }
            if (menu.isVisible()) {
                menu.handleEvent(event, window);
            }
        }

        float dt = clock.restart().asSeconds();

        if (!gameOver && !menu.isVisible()) {
            player.update(dt, window);

            for (auto& enemy : enemies) {
                if (enemy.isAlive()) {
                    enemy.update(player.getPosition(), level, dt);
                    
                    auto newBullets = enemy.shootFireballs(player.getPosition(), dt);
                    enemyBullets.insert(enemyBullets.end(), newBullets.begin(), newBullets.end());
                }
            }
        }
        
        audio.update();
        ui.update(dt);
        ui.setProgress(totalKilled, totalToKill);

        // --------------------------------------------------
        // COLISIONES
        // --------------------------------------------------

        // 1) Colisión bala-enemigo
        for (auto& enemy : enemies) {
            if (!enemy.isAlive()) continue;
            for (auto it = player.getBullets().begin(); it != player.getBullets().end(); ) {
                if (it->getBounds().intersects(enemy.getBounds())) {
                    // 1.a) La bala golpea al enemigo
                    enemy.applyDamage(1, it->getPosition());
                    it = player.getBullets().erase(it);
                } else {
                    ++it;
                }
            }
        }

        // 2) Colisión enemigo-jugador
        bool hitThisFrame = false;
        sf::FloatRect reducedPlayerBounds = player.getBounds();

        for (auto& enemy : enemies) {
            if (!enemy.isAlive()) continue;
            if (hitThisFrame) break;

            if (enemy.getBounds().intersects(reducedPlayerBounds)) {
                if (!player.isInvulnerable()) {
                    player.applyDamage(1, enemy.getPosition());
                    ui.setLives(player.getLives());
                }
                hitThisFrame = true;
                break;
            }
}
        
        // 3) Colisión moneda
        for (auto& coin : coins) {
            if (coin.checkCollision(player.getBounds())){
                ui.setMoney(ui.getMoney() + 1);
            }
        }

        // 4) Colisión bala enemiga - jugador
        for (auto it = enemyBullets.begin(); it != enemyBullets.end(); ) {
            if (it->getBounds().intersects(reducedPlayerBounds)) {
                if (!player.isInvulnerable()) {
                    player.applyDamage(it->getDamage(), it->getPosition());
                    ui.setLives(player.getLives());
                }
                it = enemyBullets.erase(it);
            } else {
                ++it;
            }
        }

        if (!gameOver && player.getLives() <= 0) {
            gameOver = true;
            gameOverClock.restart();
        }

        repelEnemiesFromPlayer(enemies, player, 16.f);

        // Añadir separación minima de 16px a cada enemigo 
        const float minDistance = 16.f;
        for (size_t i = 0; i < enemies.size(); ++i) {
            for (size_t j = i + 1; j < enemies.size(); ++j) {
                if (!enemies[i].isAlive() || !enemies[j].isAlive()) continue;
        
                sf::Vector2f posA = enemies[i].getPosition();
                sf::Vector2f posB = enemies[j].getPosition();
                sf::Vector2f delta = posB - posA;
                float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        
                if (distance < minDistance && distance > 0.0f) {
                    // Normaliza el vector y empuja a ambos enemigos en direcciones opuestas
                    sf::Vector2f push = (delta / distance) * ((minDistance - distance) / 2.f);
                    enemies[i].move(-push);  // Desplaza A en dirección opuesta
                    enemies[j].move(push);   // Desplaza B hacia el otro lado
                }
            }
        }

        // --------------------------------------------------
        // ELIMINAR ENEMIGOS MUERTOS Y CONTAR KILLS
        // --------------------------------------------------

        for (auto it = enemies.begin(); it != enemies.end(); ) {
            if (!it->isAlive()) {
                if (rand() % 100 < 20) { // 20% de probabilidad
                    coins.emplace_back(it->getPosition(), resources.coinTexture, &audio);
                }
                it = enemies.erase(it);
                totalKilled += 1;
            } else {
                ++it;
            }
        }

        enemyBullets.erase(
            std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                [&](const Bullet& b) {
                    sf::Vector2f pos = b.getPosition();
                    return pos.x < 0 || pos.x > 320 || pos.y < 0 || pos.y > 176;
                }),
            enemyBullets.end()
        );

        // --------------------------------------------------
        // GESTIÓN DE OLA / NIVEL
        // --------------------------------------------------

        if (totalKilled >= totalToKill) {
            if (level == 1) {
            setLevel(2, ui);
            
            currentWave = 1;
            totalKilled = 0;
            totalToKill = 30; 

            enemies.clear();       
            spawnWave(currentWave);
        } else if (level == 2) {
            setLevel(3, ui);

            currentWave = 1;
            totalKilled = 0;
            totalToKill = 50; 

            enemies.clear();              
            spawnWave(currentWave);   
        } else if (level == 3) {
            setLevel(4, ui);

            currentWave = 1;
            totalKilled = 0;
            totalToKill = 100; 

            enemies.clear();             
            spawnWave(currentWave); 
        }
        } else if (enemies.empty()){
            currentWave += 1;
            spawnWave(currentWave);
        }

        // --------------------------------------------------
        // DIBUJADO
        // --------------------------------------------------

        window.clear(sf::Color::Black);
        window.setMouseCursorVisible(false);
        window.setMouseCursorGrabbed(true);

        float time = shaderClock.getElapsedTime().asSeconds();
        dynamicBackground.update(time, window);
        dynamicBackground.draw(window);

        coins.erase(
            std::remove_if(coins.begin(), coins.end(), [](const Money& m) { return m.isCollected(); }),
            coins.end()
        );

        for (auto& coin : coins)
            coin.update(dt);

        for (auto& coin : coins)
            coin.draw(window);

        window.draw(grasswallSprite);

        for (auto& bullet : enemyBullets) {
            bullet.update(dt);      
            bullet.draw(window); 
    }

        for (auto& enemy : enemies) {
                if (enemy.isAlive())
                    enemy.draw(window);
            }

        player.draw(window);

        ui.draw(window);

        if (gameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                enemies.clear();
                return 1;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
                return 0; 
             }
            ui.showGameOver();
            enemies.clear();
        }
            if (menu.isVisible()) {
                audio.setVolume(
                    menu.getMasterVolume(),
                    menu.getMusicVolume(),
                    menu.getSFXVolume()
                );

                menu.updateCursorPosition(window);
                window.setMouseCursorVisible(false);
                
                menu.draw(window);
                window.display();
                continue; 
        }

        window.display();
    }
         return 0;
}