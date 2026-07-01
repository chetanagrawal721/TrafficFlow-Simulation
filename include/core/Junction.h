#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "TrafficController.h"
#include "TrafficLight.h"

class Junction {
private:
    sf::Vector2f position;
    TrafficController controller;
    std::vector<TrafficLight*> lights;
    
public:
    Junction(const sf::Vector2f& pos) : position(pos) {}
    
    void addLight(TrafficLight* light) {
        lights.push_back(light);
    }
    
    TrafficController& getController() {
        return controller;
    }
    
    const std::vector<TrafficLight*>& getLights() const {
        return lights;
    }
    
    void update(float dt) {
        controller.update(dt, lights);
    }
    
    void draw(sf::RenderWindow& window) {
        for (auto* light : lights) {
            light->draw(window);
        }
    }
};
