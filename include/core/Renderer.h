#pragma once

#include <SFML/Graphics.hpp>
#include "Junction.h"
#include "VehicleManager.h"
#include "utils/Constants.h"

class Renderer {
private:
    sf::RenderWindow& window;
    sf::RectangleShape roadHorizontal;
    sf::RectangleShape roadVertical;
    sf::RectangleShape junctionBox;
    
public:
    Renderer(sf::RenderWindow& win) : window(win) {
        setupRoads();
    }
    
    void setupRoads() {
        // Horizontal road
        roadHorizontal.setSize({Constants::WINDOW_WIDTH, Constants::ROAD_WIDTH});
        roadHorizontal.setPosition({0.f, Constants::JUNCTION_CENTER_Y - Constants::ROAD_WIDTH / 2.f});
        roadHorizontal.setFillColor(sf::Color(80, 80, 80));
        
        // Vertical road
        roadVertical.setSize({Constants::ROAD_WIDTH, Constants::WINDOW_HEIGHT});
        roadVertical.setPosition({Constants::JUNCTION_CENTER_X - Constants::ROAD_WIDTH / 2.f, 0.f});
        roadVertical.setFillColor(sf::Color(80, 80, 80));
        
        // Junction box
        junctionBox.setSize({Constants::JUNCTION_SIZE, Constants::JUNCTION_SIZE});
        junctionBox.setOrigin({Constants::JUNCTION_SIZE / 2.f, Constants::JUNCTION_SIZE / 2.f});
        junctionBox.setPosition({Constants::JUNCTION_CENTER_X, Constants::JUNCTION_CENTER_Y});
        junctionBox.setFillColor(sf::Color(60, 60, 60));
        junctionBox.setOutlineColor(sf::Color::White);
        junctionBox.setOutlineThickness(2.f);
    }
    
    void drawAll(Junction& junction, VehicleManager& vehicleManager) {
        window.clear(sf::Color(20, 20, 20));
        window.draw(roadHorizontal);
        window.draw(roadVertical);
        window.draw(junctionBox);
        
        for (auto* light : junction.getLights()) {
            light->draw(window);
        }
        
        vehicleManager.draw(window);
    }
};
