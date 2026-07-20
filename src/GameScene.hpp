#pragma once
#include "SGEL.hpp"
#include "utils/DungeonGenerator.hpp"

class GameScene : public Scene {
    Dungeon dungeon;

public:
    ~GameScene() noexcept override;

    bool init() override;
    void exit() override;

    SDL_AppResult update(float deltaTime) override;
    SDL_AppResult eventHandler(const SDL_Event *event) override;
    void sDoAction(const Action &action) override;
    void sRender(SDL_Renderer *renderer) override;

    std::shared_ptr<Scene> changeScene() override;
};
