#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Object {
public:
    sf::Vector2f position;
    std::weak_ptr<sf::Sprite> sprite;
public:
    virtual ~Object() = default;
    virtual void update(double dt) = 0;
    virtual void render(sf::RenderWindow &window) const = 0;

};
