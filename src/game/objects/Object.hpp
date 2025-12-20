#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <utility>

class Object {
public:
    sf::Vector2f position;
    sf::Sprite sprite;
public:
    Object(sf::Sprite sprite);
    virtual ~Object() = default;
    virtual void update(double dt) = 0;
    virtual void draw(sf::RenderWindow &window) const = 0;

};

inline Object::Object(sf::Sprite sprite): sprite(std::move(sprite)) {
}
