#pragma once
#include <memory>

class State {
public:
    virtual ~State() = default;

    virtual std::unique_ptr<State> update(double dt) = 0;
    virtual void render() = 0;
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
};

class PlayerState : public State {
public:
    explicit PlayerState(sf::Vector2f &position);
    virtual std::unique_ptr<State> handleInput(const std::optional<sf::Event> &event) = 0;

protected:
    sf::Vector2f& position;
};

inline PlayerState::PlayerState(sf::Vector2f &position): position(position) {
}
