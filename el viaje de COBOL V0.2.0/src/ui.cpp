#include "ui.hpp"
#include "cmath" 
#include <sstream>

UI::UI() {
    uiBgTexture.loadFromFile("assets/sprites/ui/panel/ui_panel.png");
    uiBackground.setTexture(uiBgTexture);
    uiBackground.setPosition(270, 0);

    font.loadFromFile("assets/fonts/GrapeSoda.ttf");

    levelText.setFont(font);
    levelText.setCharacterSize(62);
    levelText.setFillColor(sf::Color::White);
    levelText.scale(0.3f, 0.3f);                          
    shadowText = levelText;
    shadowText.setFillColor(sf::Color(0, 0, 55, 165));

    moneyText.setFont(font);
    moneyText.setCharacterSize(52);
    moneyText.setFillColor(sf::Color::White);
    moneyText.scale(0.3f, 0.3f);                         
    moneyShadow = moneyText;
    moneyShadow.setFillColor(sf::Color(0, 0, 55, 165));


    skullCounterText.setFont(font);
    skullCounterText.setCharacterSize(62);
    skullCounterText.setFillColor(sf::Color::White);
    skullCounterText.setPosition(progressBarPosition.x - 38.f, progressBarPosition.y - 2.f);
    skullCounterText.setString("0x");
    skullCounterShadow = skullCounterText;
    skullCounterShadow.setFillColor(sf::Color(0, 0, 55, 150));

    heartTexture.loadFromFile("assets/sprites/ui/heart.png");
    setLevel(1);
    setLives(lives); 

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(62);
    gameOverText.setString("GAME OVER");
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setScale(0.65f, 0.65f);
    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(bounds.left + bounds.width / 2.f,
                        bounds.top + bounds.height / 2.f);
    gameOverText.setPosition(160.f, 10.f); 
    gameOverShadow = gameOverText;
    gameOverShadow.setFillColor(sf::Color(0, 0, 0, 150));
    gameOverShadow.setPosition(gameOverText.getPosition().x + 2.f,
                            gameOverText.getPosition().y + 2.f);

    playAgainText.setFont(font);
    playAgainText.setCharacterSize(72);
    playAgainText.setString("Pulsa ENTER o X para volver a jugar.");
    playAgainText.setFillColor(sf::Color::White);
    playAgainText.setScale(0.22f, 0.22f);
    sf::FloatRect centerPlayAgain = playAgainText.getLocalBounds();
    playAgainText.setOrigin(bounds.left + bounds.width / 2.f,
                        bounds.top + bounds.height / 2.f);
                        playAgainText.setPosition(160.f, 10.f); 
    playAgainShadow = playAgainText;
    playAgainShadow.setFillColor(sf::Color(0, 0, 0, 150));
    playAgainShadow.setPosition(playAgainText.getPosition().x + 2.f,
                            playAgainShadow.getPosition().y + 2.f);             
}

void UI::setLevel(int level) {
    currentLevel = level;
    levelText.setString("Nivel " + std::to_string(level));
    sf::FloatRect bounds = levelText.getLocalBounds();
    levelText.setOrigin(bounds.left + bounds.width  / 2.f,
                        bounds.top  + bounds.height / 2.f);
    textBaseX = 15.f + (bounds.width  * 0.22f) / 2.f;
    textBaseY = 10.f + (bounds.height * 0.22f) / 2.f;
    
    levelText.setPosition(textBaseX, textBaseY);

    shadowText.setString(levelText.getString());
    shadowText.setOrigin(bounds.left + bounds.width  / 2.f,
                         bounds.top  + bounds.height / 2.f);
    shadowText.setPosition(textBaseX + 1.f, textBaseY + 1.5f);
}

void UI::setLives(int count) {
    lives = count;
    hearts.clear();

    if (lives <= 0) return;

    for (int i = 0; i < lives; ++i) {
        sf::Sprite heart;
        heart.setTexture(heartTexture);
        heart.setScale(1.2f, 1.2f);

        float x = -10.f;
        float y = -10.f;

        sf::FloatRect bounds = heart.getLocalBounds();
        heart.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        heart.setPosition(x, y);
        hearts.push_back(heart);
    }
}

void UI::setMoney(int money) {
    currentMoney = money;
    moneyText.setString(std::to_string(money));

    sf::FloatRect bounds = moneyText.getLocalBounds();
    moneyText.setOrigin(bounds.left + bounds.width  / 2.f,
                        bounds.top  + bounds.height / 2.f);

    moneyBaseX = 15.f + (bounds.width  * 0.22f) / 2.f;
    moneyBaseY = 10.f + (bounds.height * 0.22f) / 2.f;

    moneyText.setPosition(moneyBaseX, moneyBaseY);

    moneyShadow.setString(moneyText.getString());
    moneyShadow.setOrigin(bounds.left + bounds.width  / 2.f,
                         bounds.top  + bounds.height / 2.f);
    moneyShadow.setPosition(moneyBaseX + 1.f, moneyBaseY + 1.5f);
}

int UI::getMoney() const {
    return currentMoney;
}
void UI::update(float dt) {
    elapsedTime += dt;

    float angle = std::sin(elapsedTime * 0.5f) * 5.f;
    float yOffset = std::sin(elapsedTime * 1.5f) * 1.5f;

    levelText.setRotation(angle);
    levelText.setPosition(textBaseX, textBaseY + yOffset);

    shadowText.setRotation(levelText.getRotation());
    shadowText.setPosition(textBaseX + 1.f, textBaseY + yOffset + 1.5f);

    moneyBaseX = 298.0f;
    moneyBaseY = 30.0f;

    float angleMoney = std::sin(elapsedTime * 0.5f) * 3.f;
    float yOffsetMoney = std::sin(elapsedTime * 1.0f) * 1.0f;

    moneyText.setRotation(angleMoney);
    moneyText.setPosition(moneyBaseX, moneyBaseY + yOffsetMoney);

    moneyShadow.setRotation(moneyText.getRotation());
    moneyShadow.setPosition(moneyBaseX + 1.f, moneyBaseY + yOffsetMoney + 1.5f);

    heartBaseX = 296.0f;
    heartBaseY = 100.f;
    
    for (size_t i = 0; i < hearts.size(); ++i) {
        size_t reverseIndex = hearts.size() - 1 - i;
        float x = heartBaseX;
        float y = heartBaseY - i * 16.f;
    
        if (lives == 1 && i == 0) {
            float shakeX = std::sin(elapsedTime * 30.f) * 0.5f;
            float shakeY = std::cos(elapsedTime * 30.f) * 0.5f;
            hearts[i].setRotation(angle);
            hearts[i].setPosition(x + shakeX, y + shakeY);
        } else {
            float heartAngle = std::sin(elapsedTime * 1.5f) * 4.5f;
            hearts[i].setRotation(heartAngle);
            hearts[i].setPosition(x, y); 
       }
    }

    if (bounceTimer > 0.f) {
        bounceTimer -= dt;

        float progress = 1.f - (bounceTimer / bounceDuration);
        float scale = 0.22f + 0.04f * std::sin(progress * 3.1415f);
        skullCounterText.setScale(scale, scale);
    } else {
        skullCounterText.setScale(0.22f, 0.22f);
    }

        skullCounterShadow = skullCounterText;
        skullCounterShadow.setFillColor(sf::Color(0, 0, 0, 150));
        skullCounterShadow.setPosition(
        skullCounterText.getPosition().x + 1.f,
        skullCounterText.getPosition().y + 1.f
    );
    skullCounterShadow.setScale(skullCounterText.getScale());


    if (showGameOverText) {
        float wobble = std::sin(elapsedTime * 1.8f) * 4.f;
        float offsetY = std::sin(elapsedTime * 3.f) * 1.8f;
    
        gameOverText.setRotation(wobble);
        gameOverText.setPosition(160.f, 60.f + offsetY);
    
        gameOverShadow.setRotation(wobble);
        gameOverShadow.setPosition(160.f + 2.f, 60.f + offsetY + 2.f);

        float wobbleAgain = std::sin(elapsedTime * 1.2f) * 2.f;
        float offsetYAgain = std::sin(elapsedTime * 1.2f) * 0.8f;

        playAgainText.setRotation(wobbleAgain);
        playAgainText.setPosition(75.f, 100.f + offsetYAgain);
    
        playAgainShadow.setRotation(wobbleAgain);
        playAgainShadow.setPosition(75.f + 2.f, 100.f + offsetYAgain + 2.f);

    }
}

void UI::loadTextures() {
    progressBgTexture.loadFromFile("assets/sprites/ui/ProgressBar/progress_bg.png");
    progressFillTexture.loadFromFile("assets/sprites/ui/ProgressBar/progress_fill.png");
    skeletonHeadTexture.loadFromFile("assets/sprites/ui/ProgressBar/progress_skeleton.png");
    
    progressBgSprite.setTexture(progressBgTexture);
    progressBgSprite.setPosition(progressBarPosition);

    progressFillSprite.setTexture(progressFillTexture);
    progressFillSprite.setPosition(progressBarPosition);

    skeletonHeadSprite.setTexture(skeletonHeadTexture);
    skeletonHeadSprite.setPosition(progressBarPosition);

}

void UI::setProgress(int killed, int total) {
    enemiesKilled = killed;
    totalToKill = total;

    if (enemiesKilled > displayedKilled) {
        bounceTimer = bounceDuration;
        displayedKilled = enemiesKilled;
    }
    skullCounterText.setString(std::to_string(enemiesKilled) + " / " + std::to_string(totalToKill));

    float progress = static_cast<float>(enemiesKilled) / totalToKill;
    progress = std::clamp(progress, 0.f, 1.f);

    const int texWidth = progressFillTexture.getSize().x;
    const int texHeight = progressFillTexture.getSize().y;
    const int marginLeft = 8; 

    int maxFillableWidth = texWidth - marginLeft;

    int visibleWidth = static_cast<int>(maxFillableWidth * progress);

    progressFillSprite.setTextureRect(sf::IntRect(texWidth - visibleWidth, 0, visibleWidth, texHeight));

    float scaleX = progressFillSprite.getScale().x;
    float scaledTexWidth = texWidth * scaleX;
    float scaledVisibleWidth = visibleWidth * scaleX;

    progressFillSprite.setPosition(
        progressBarPosition.x + scaledTexWidth - scaledVisibleWidth,
        progressBarPosition.y
    );
}

void UI::drawProgressBar(sf::RenderWindow& window) {
    window.draw(progressBgSprite);
    window.draw(progressFillSprite);
    window.draw(skeletonHeadSprite);
    window.draw(skullCounterShadow);
    window.draw(skullCounterText);
}

void UI::draw(sf::RenderWindow& window) {
    window.draw(uiBackground);

    window.draw(shadowText);
    window.draw(levelText);

    window.draw(moneyShadow);
    window.draw(moneyText);

    for (const auto& heart : hearts) {
        window.draw(heart);
    }

    drawProgressBar(window);

    if (showGameOverText) {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(320, 176));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        window.draw(gameOverShadow);
        window.draw(gameOverText);

        window.draw(playAgainShadow);
        window.draw(playAgainText);
    }
}

void UI::showGameOver() {
    showGameOverText = true;
}