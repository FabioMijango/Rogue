#pragma once
#include "Assets.hpp"
#include "../Bible.hpp"

class AssetsUtils {
    AssetsUtils() = default;
public:
    static bool loadAssets() {
        Assets& assets = Assets::Instance();
        const SDL_Texture *roguesTex = assets.loadTexture(bb::rogueRes.name, bb::rogueRes.path, false);
        if (!roguesTex) return false;

        const SDL_Texture *tileTex = assets.loadTexture(bb::tileRes.name, bb::tileRes.path, false);
        if (!tileTex) return false;

        Animation* anim =  assets.loadAnimation("player", bb::rogueRes.name,{ 0, 0, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
        if (!anim) return false;


        // Temp floor texture
        Animation* floorAnim =  assets.loadAnimation(bb::ANIMID_FLOOR, bb::tileRes.name, { 0 , 7 * bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
        if (!floorAnim) return false;

        // Temp wall texture
        Animation* sideWallAnim =  assets.loadAnimation(bb::ANIMID_SIDE_WALL, bb::tileRes.name, { 0 , 0, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
        if (!sideWallAnim) return false;

        Animation* topWallAnim =  assets.loadAnimation(bb::ANIMID_TOP_WALL, bb::tileRes.name, { 0 , 1 * bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
        if (!topWallAnim) return false;

        return true;
    }
};