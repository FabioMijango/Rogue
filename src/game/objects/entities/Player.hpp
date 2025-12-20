#pragma once
#include "Entity.hpp"

class Player : public Entity {
public:
    Player(sf::Sprite sprite);
    void handleInput(const std::optional<sf::Event> &event);
    void update(double dt) override;
    void draw(sf::RenderWindow &window) const override;
};
