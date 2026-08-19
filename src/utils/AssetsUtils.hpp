#pragma once
#include "Animation.hpp"
#include "Assets.hpp"
#include "../Bible.hpp"

class AssetsUtils {
    AssetsUtils() = default;
public:
    static bool loadAssets() {
        static bool loaded = false;
        if (loaded) return true;
        Assets& assets = Assets::Instance();

        // Textures
        const SDL_Texture *roguesTex = assets.loadTexture(bb::rogueRes.name, bb::rogueRes.path, false);
        if (!roguesTex) return false;

        const SDL_Texture *monsterTex = assets.loadTexture(bb::monsterRes.name, bb::monsterRes.path, false);
        if (!monsterTex) return false;

        const SDL_Texture *tileTex = assets.loadTexture(bb::tileRes.name, bb::tileRes.path, false);
        if (!tileTex) return false;

        const SDL_Texture *heartTex = assets.loadTexture( bb::heartRes.name, bb::heartRes.path, false);
        if (!heartTex) return false;

        const SDL_Texture *hitText = assets.loadTexture(bb::hitRes.name, bb::hitRes.path, false);
        if (!hitText) return false;


        // Animations
        Animation* playerAnim =  assets.loadAnimation(bb::Anim::ID_PLAYER, bb::rogueRes.name,{ 0, 0, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
        if (!playerAnim) return false;

        Animation* enemyAnim = assets.loadAnimation(bb::Anim::ID_ENEMY, bb::monsterRes.name, { 0, 0, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
        if (!enemyAnim) return false;

        // Temp floor texture
        Animation* floorAnim =  assets.loadAnimation(bb::Anim::ID_FLOOR, bb::tileRes.name, { 0 , 7 * bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
        if (!floorAnim) return false;

        // Temp wall texture
        Animation* sideWallAnim =  assets.loadAnimation(bb::Anim::ID_SIDE_WALL, bb::tileRes.name, { 0 , 0, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
        if (!sideWallAnim) return false;

        Animation* topWallAnim =  assets.loadAnimation(bb::Anim::ID_TOP_WALL, bb::tileRes.name, { 1 * bb::ASSETS_TILE_SIZE , 0, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
        if (!topWallAnim) return false;

        Animation* stairsAnim = assets.loadAnimation(bb::Anim::ID_STAIRS, bb::tileRes.name, {  7 * bb::ASSETS_TILE_SIZE , 16 * bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
        if(!stairsAnim) return false;

        Animation* heartAnim = assets.loadAnimation(bb::Anim::ID_HEART, bb::heartRes.name, {0,0,  12 * bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE}, 12, 5);
        if(!heartAnim) return false;

        Animation* hitAnim =  assets.loadAnimation(bb::Anim::ID_HIT, bb::hitRes.name, {0,0, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE});
        if (!hitAnim) return false;

        loaded = true;
        return true;
    }
};
