#pragma once

#include <optional>
#include <SFML/Window/Event.hpp>

#include "State.hpp"

class PlayerNormal : public PlayerState {
public:
    explicit PlayerNormal(sf::Vector2f &position);
    std::unique_ptr<State> handleInput(const std::optional<sf::Event> &event) override;
    std::unique_ptr<State> update(double dt) override;
    void render() override;
    void onEnter() override;
    void onExit() override;

private:
    const float velocity = 200.0f;
    sf::Vector2f currentVelocity;
};
