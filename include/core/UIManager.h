#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "utils/Constants.h"

class UIManager {
private:
    sf::Font font;
    bool fontLoaded = false;
    
public:
    UIManager() = default;
    
    bool loadFont() {
        // Try multiple font paths
        if (font.openFromFile("arial.ttf")) {
            fontLoaded = true;
            return true;
        }
        if (font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            fontLoaded = true;
            return true;
        }
        if (font.openFromFile("C:/Windows/Fonts/calibrib.ttf")) {
            fontLoaded = true;
            return true;
        }
        if (font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            fontLoaded = true;
            return true;
        }
        return false;
    }
    
    void update(float /*dt*/) {}
    
    void draw(sf::RenderWindow& window, 
              int vehicleCount, 
              int emergencyCount,
              int greenLane, 
              bool emergencyActive) {
        
        if (!fontLoaded) return;
        
        // ===== MAIN HUD (Top-Left) =====
        std::ostringstream oss;
        oss << "TRAFFIC CONTROL\n";
        oss << "====================\n";
        oss << "Vehicles:   " << std::setw(3) << vehicleCount << "\n";
        oss << "Emergency:  " << std::setw(3) << emergencyCount << "\n";
        oss << "Lane:       " << std::setw(3) << greenLane << "\n";
        oss << "Status:     " << (emergencyActive ? "EMERGENCY!" : "NORMAL") << "\n";
        
        sf::Text mainHUD(font, oss.str(), 14);
        mainHUD.setFillColor(sf::Color::White);
        mainHUD.setPosition({10.f, 10.f});
        window.draw(mainHUD);
        
        // ===== LANE STATUS (Top-Right) =====
        std::ostringstream lanes;
        lanes << "LANE STATUS\n";
        lanes << "===========\n";
        lanes << "0: " << (greenLane == 0 && !emergencyActive ? "GREEN" : 
                           emergencyActive && greenLane == 0 ? "EMERG-GREEN" : "RED") << "\n";
        lanes << "1: " << (greenLane == 1 && !emergencyActive ? "GREEN" :
                           emergencyActive && greenLane == 1 ? "EMERG-GREEN" : "RED") << "\n";
        lanes << "2: " << (greenLane == 2 && !emergencyActive ? "GREEN" :
                           emergencyActive && greenLane == 2 ? "EMERG-GREEN" : "RED") << "\n";
        lanes << "3: " << (greenLane == 3 && !emergencyActive ? "GREEN" :
                           emergencyActive && greenLane == 3 ? "EMERG-GREEN" : "RED") << "\n";
        
        sf::Text laneStatus(font, lanes.str(), 12);
        laneStatus.setFillColor(emergencyActive ? sf::Color::Red : sf::Color::Cyan);
        laneStatus.setPosition({Constants::WINDOW_WIDTH - 200.f, 10.f});
        window.draw(laneStatus);
        
        // ===== EMERGENCY ALERT (Center) =====
        if (emergencyActive) {
            std::ostringstream alert;
            alert << "EMERGENCY VEHICLE IN PROGRESS - LANE " << greenLane;
            
            sf::Text emergencyText(font, alert.str(), 18);
            emergencyText.setFillColor(sf::Color::Red);
            emergencyText.setStyle(sf::Text::Bold);
            
           
            float textWidth = emergencyText.getLocalBounds().size.x;
            emergencyText.setPosition({
                Constants::WINDOW_WIDTH / 2.f - textWidth / 2.f,
                Constants::WINDOW_HEIGHT / 2.f - 50.f
            });
            window.draw(emergencyText);
        }
        
        // ===== CONTROLS (Bottom) =====
        std::ostringstream controls;
        controls << "A/S/D/W=Normal  E/F/H/Q=Emergency  R=Reset  ESC=Exit";
        
        sf::Text controlText(font, controls.str(), 11);
        controlText.setFillColor(sf::Color::Yellow);
        controlText.setPosition({10.f, Constants::WINDOW_HEIGHT - 30.f});
        window.draw(controlText);
    }
};
