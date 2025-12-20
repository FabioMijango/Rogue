#pragma once
#include "../Object.hpp"
#include "states/State.hpp"

class Entity : public Object {
protected:
    uint16_t maxHealth = 1;
    uint16_t health = maxHealth;
    float velocity = 50.f;
    sf::Vector2f currentVelocity;
    std::unique_ptr<State> currentState;
    std::unique_ptr<State> previousState;

public:
    explicit Entity(const sf::Sprite &sprite);
    void update(double dt) override = 0;
    void draw(sf::RenderWindow &window) const override = 0;
};

inline Entity::Entity(const sf::Sprite &sprite): Object(sprite) {
}
