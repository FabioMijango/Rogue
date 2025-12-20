#pragma once

class State {
public:
    virtual ~State() = default;

    virtual void update(double dt) = 0;
    virtual void render() = 0;
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
};