#pragma once

#include "entities/Vehicle.h"

// Abstract Decorator
class VehicleDecorator : public Vehicle {
protected:
    Vehicle* wrappedVehicle;
    
public:
    VehicleDecorator(Vehicle* vehicle) : wrappedVehicle(vehicle) {}
    
    virtual ~VehicleDecorator() {
        delete wrappedVehicle;
    }
    
    Vector2D getPosition() const override {
        return wrappedVehicle->getPosition();
    }
    
    void update(float deltaTime) override {
        wrappedVehicle->update(deltaTime);
    }
    
    void draw(sf::RenderWindow& window) override {
        wrappedVehicle->draw(window);
    }
    
    bool getIsEmergency() const override {
        return wrappedVehicle->getIsEmergency();
    }
    
    int getLaneID() const override {
        return wrappedVehicle->getLaneID();
    }
    
    void checkTrafficLight(const TrafficLight& light) override {
        wrappedVehicle->checkTrafficLight(light);
    }
};

// Eco-Badge Decorator
class EcoBadgeDecorator : public VehicleDecorator {
public:
    EcoBadgeDecorator(Vehicle* vehicle) : VehicleDecorator(vehicle) {
        std::cout << "[DECORATOR] Eco-Badge added to vehicle\n";
    }
    
    void update(float deltaTime) override {
        wrappedVehicle->update(deltaTime * 1.05f);
    }
    
    void draw(sf::RenderWindow& window) override {
        wrappedVehicle->draw(window);
    }
};

// Priority Badge Decorator
class PriorityBadgeDecorator : public VehicleDecorator {
public:
    PriorityBadgeDecorator(Vehicle* vehicle) : VehicleDecorator(vehicle) {
        std::cout << "[DECORATOR] Priority Badge added to vehicle\n";
    }
    
    void checkTrafficLight(const TrafficLight& light) override {
        if (light.getState() == LightState::Yellow) {
            std::cout << "[PRIORITY] Vehicle can proceed through yellow\n";
        }
        wrappedVehicle->checkTrafficLight(light);
    }
};

// Heavy Load Decorator
class HeavyLoadDecorator : public VehicleDecorator {
public:
    HeavyLoadDecorator(Vehicle* vehicle) : VehicleDecorator(vehicle) {
        std::cout << "[DECORATOR] Heavy Load added - 10% speed reduction\n";
    }
    
    void update(float deltaTime) override {
        wrappedVehicle->update(deltaTime * 0.9f);
    }
};
