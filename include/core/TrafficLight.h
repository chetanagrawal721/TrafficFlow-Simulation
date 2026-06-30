#pragma once

#include <SFML/Graphics.hpp>
#include "utils/Vector2D.h"

enum class LightState {
    Red = 0,
    Yellow = 1,
    Green = 2
};

class TrafficLight {
private:
    float x, y;
    int laneID;
    LightState state;
    sf::CircleShape lightShape;
    sf::CircleShape outerRing;
    
public:
    TrafficLight(float px, float py, int lane)
        : x(px), y(py), laneID(lane), state(LightState::Red) {
        
        // Main light
        lightShape.setRadius(15.f);
        lightShape.setFillColor(sf::Color::Red);
        lightShape.setOutlineThickness(2.f);
        lightShape.setOutlineColor(sf::Color::White);
        
        // Outer ring for emphasis
        outerRing.setRadius(18.f);
        outerRing.setOrigin({18.f, 18.f});
        outerRing.setFillColor(sf::Color::Transparent);
        outerRing.setOutlineThickness(1.f);
        outerRing.setOutlineColor(sf::Color::White);
    }
    
    void setState(LightState newState) {
        state = newState;
        switch (state) {
            case LightState::Red:
                lightShape.setFillColor(sf::Color::Red);
                break;
            case LightState::Yellow:
                lightShape.setFillColor(sf::Color::Yellow);
                break;
            case LightState::Green:
                lightShape.setFillColor(sf::Color::Green);
                break;
        }
    }
    
    void update() {}
    
    void draw(sf::RenderWindow& window) {
        lightShape.setPosition(sf::Vector2f(x - 15.f, y - 15.f));
        window.draw(lightShape);
        
        outerRing.setPosition(sf::Vector2f(x, y));
        window.draw(outerRing);
    }
    
    LightState getState() const { return state; }
    int getLaneID() const { return laneID; }
    Vector2D getPosition() const { return Vector2D(x, y); }
    bool isGreen() const { return state == LightState::Green; }
    bool isRed() const { return state == LightState::Red; }
};
