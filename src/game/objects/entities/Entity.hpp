#pragma once
#include "../Object.hpp"
#include "states/State.hpp"

class Entity : public Object {
protected:
    uint16_t maxHealth = 1;
    uint16_t health = maxHealth;
    std::unique_ptr<State> currentState;
    std::unique_ptr<State> previousState;

public:
    void update(double dt) override = 0;
    void draw(sf::RenderWindow &window) const override = 0;
};
