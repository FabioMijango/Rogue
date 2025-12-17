#pragma once
#include "Screen.hpp"
#include "StartScreen.hpp"
#include "../../core/ResourceManager.hpp"

struct Context {
    ResourceManager *resourceManager;
    sf::RenderWindow *window;
};

class ScreenManager {
public:
    ScreenManager() = default;
    void init(Context ctx);
    void handleInput(const std::optional<sf::Event> &event) const;
    void update(double dt) const;
    void render() const;
    void changeScreen(std::unique_ptr<Screen> newScreen);

private:
    Context context;
    std::unique_ptr<Screen> currentScreen;
    std::unique_ptr<Screen> previousScreen;
};
