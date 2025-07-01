#include "resources.hpp"
#include <iostream>

void GameResources::load() {    
    grasswallTexture.resize(4);
    for (int i = 0; i < 4; ++i) {
        grasswallTexture[i].loadFromFile("assets/sprites/backgrounds/grasswall_0" + std::to_string(i + 1) + ".png");
    }

    blueBulletTexture.loadFromFile("assets/sprites/bullets/blue_bullet_sheet.png");
    firedBulletTexture.loadFromFile("assets/sprites/bullets/fired_bullet_sheet.png");

    skeletonTexture.loadFromFile("assets/sprites/enemies/skeleton_spritesheet.png");
    firedSkeletonTexture.loadFromFile("assets/sprites/enemies/fired_skeleton_spritesheet.png");
    pitcherSkeletonTexture.loadFromFile("assets/sprites/enemies/pitcher_skeleton_spritesheet.png");

    coinTexture.loadFromFile("assets/sprites/money/coin_spritesheet.png");
    powerUpTexture.loadFromFile("assets/sprites/bullets/upgrade_bullet_sheet.png");

    textureMenuBackground.loadFromFile("assets/sprites/menu/menu_bg.png");
    textureMenuTitle.loadFromFile("assets/sprites/menu/menu_title.png");
    textureMuteMusic.loadFromFile("assets/sprites/menu/btn_mute_music.png");
    textureUnmuteMusic.loadFromFile("assets/sprites/menu/btn_unmute_music.png");
    textureMuteSFX.loadFromFile("assets/sprites/menu/btn_mute_sfx.png");
    textureUnmuteSFX.loadFromFile("assets/sprites/menu/btn_unmute_sfx.png");
    textureContinue.loadFromFile("assets/sprites/menu/btn_continue.png");
    textureExit.loadFromFile("assets/sprites/menu/btn_exit.png");
    textureCursor.loadFromFile("assets/sprites/menu/cursor.png");
}
