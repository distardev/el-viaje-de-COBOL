#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class UI {
public:
    UI();                                           

    void setLevel(int level);                       
    void setLives(int lives);                       
    void setMoney(int money);
    int getMoney() const;

    void update(float dt);                       
    void draw(sf::RenderWindow& window);           

    void setProgress(int killed, int total);        
    void drawProgressBar(sf::RenderWindow& window); 
    void loadTextures();

    void showGameOver();
    
private:
    sf::Sprite uiBackground;
    sf::Texture uiBgTexture;

    sf::Font font;                                  
    sf::Text levelText;                             
    sf::Text shadowText;                            

    sf::Text moneyText;                            
    sf::Text moneyShadow;                            

    float textBaseX = 0.f;
    float textBaseY = 0.f;

    float moneyBaseX = 0.f;
    float moneyBaseY = 0.f;

    bool mostrarPrimerTexto = true;
    sf::Clock relojCambioTexto;

    std::string texto1 = "Pulsa las teclas WASD o las crucetas para moverte.";
    std::string texto2 = "Pulsa CLICK IZQUIERDO o X para disparar.";

    int currentLevel = 1;                           
    int lives = 3;                                 
    int currentMoney = 0;                             

    sf::Texture heartTexture;                      
    std::vector<sf::Sprite> hearts;                 

    float heartBaseX = 0.f;                         
    float heartBaseY = 8.f;           

    float elapsedTime = 0.f;                        

    int totalToKill = 15;
    int enemiesKilled = 0;
    int totalKilledGlobal = 0;

    sf::Texture progressBgTexture;
    sf::Texture progressFillTexture;
    sf::Texture skeletonHeadTexture;
    sf::Sprite progressBgSprite;
    sf::Sprite progressFillSprite;
    sf::Sprite skeletonHeadSprite;

    float maxProgressBarWidth = 200.0f;                       
    sf::Vector2f progressBarPosition = {200.f, 160.f};           
    sf::Text skullCounterText;
    sf::Text skullCounterShadow;                            

    int displayedKilled = 0;
    float bounceTimer = 0.f;
    const float bounceDuration = 0.2f;

    sf::Text gameOverText;
    sf::Text gameOverShadow;
    bool showGameOverText = false;

    sf::Text playAgainText;
    sf::Text playAgainShadow;                          
};
