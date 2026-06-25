#pragma once
#include <SFML/System/Clock.hpp>

class Timer {
private:
    sf::Clock clock;

public:
    void restart() { clock.restart(); }
    float getElapsedSeconds() const { return clock.getElapsedTime().asSeconds(); }
};
