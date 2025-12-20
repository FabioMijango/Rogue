#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "entities/Entity.hpp"
#include "entities/Player.hpp"

class EntityManager {
public:
    void init(sf::Sprite playerSprite);
    void handleInput(const std::optional<sf::Event> &event);
    void update(double dt);
    void draw(sf::RenderWindow &window) const;
private:
    std::unique_ptr<std::vector<Entity>> enemies;
    std::unique_ptr<Player> player;
};
