#pragma once

#include "entities/Vehicle.h"
#include "entities/NormalVehicle.h"
#include "entities/EmergencyVehicle.h"
#include "utils/Vector2D.h"

class VehicleFactory {
public:
    static Vehicle* createNormalVehicle(Vector2D position, int lane) {
        try {
            auto* vehicle = new NormalVehicle(position, lane);
            std::cout << "[FACTORY] Normal Vehicle created at Lane " << lane << "\n";
            return vehicle;
        } catch (const std::exception& e) {
            std::cout << "[FACTORY ERROR] " << e.what() << "\n";
            return nullptr;
        }
    }
    
    static Vehicle* createAmbulance(Vector2D position, int lane) {
        try {
            auto* vehicle = new EmergencyVehicle(position, VehicleType::Ambulance, 
                                                 sf::Color::White, lane, 1);
            std::cout << "[FACTORY] Ambulance created at Lane " << lane << "\n";
            return vehicle;
        } catch (const std::exception& e) {
            std::cout << "[FACTORY ERROR] " << e.what() << "\n";
            return nullptr;
        }
    }
    
    static Vehicle* createFireTruck(Vector2D position, int lane) {
        try {
            auto* vehicle = new EmergencyVehicle(position, VehicleType::FireTruck, 
                                                 sf::Color::Red, lane, 2);
            std::cout << "[FACTORY] FireTruck created at Lane " << lane << "\n";
            return vehicle;
        } catch (const std::exception& e) {
            std::cout << "[FACTORY ERROR] " << e.what() << "\n";
            return nullptr;
        }
    }
    
    static Vehicle* createPoliceVehicle(Vector2D position, int lane) {
        try {
            auto* vehicle = new EmergencyVehicle(position, VehicleType::Police, 
                                                 sf::Color::Blue, lane, 3);
            std::cout << "[FACTORY] Police Vehicle created at Lane " << lane << "\n";
            return vehicle;
        } catch (const std::exception& e) {
            std::cout << "[FACTORY ERROR] " << e.what() << "\n";
            return nullptr;
        }
    }
    
    static Vehicle* createVIPVehicle(Vector2D position, int lane) {
        try {
            auto* vehicle = new EmergencyVehicle(position, VehicleType::VIP, 
                                                 sf::Color::Black, lane, 4);
            std::cout << "[FACTORY] VIP Vehicle created at Lane " << lane << "\n";
            return vehicle;
        } catch (const std::exception& e) {
            std::cout << "[FACTORY ERROR] " << e.what() << "\n";
            return nullptr;
        }
    }
};
