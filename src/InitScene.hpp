#pragma once

#include "Scene.hpp"

class InitScene : public Scene {
public:
    bool init(void** screenData) override;
    void exit() override;

    SDL_AppResult update(float deltaTime) override;
    SDL_AppResult eventHandler(const SDL_Event *event) override;
    void sDoAction(const Action &action) override;
    void sRender(SDL_Renderer *renderer) override;

    std::shared_ptr<Scene> changeScene() override;
};
