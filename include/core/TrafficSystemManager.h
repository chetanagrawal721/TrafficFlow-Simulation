#pragma once

#include <memory>
#include <iostream>

class Junction;
class VehicleManager;

class TrafficSystemManager {
private:
    static TrafficSystemManager* instance;
    
    Junction* mainJunction = nullptr;
    VehicleManager* vehicleManager = nullptr;
    
    bool isRunning = false;
    float simulationTime = 0.0f;
    
    TrafficSystemManager() {
        std::cout << "[SINGLETON] TrafficSystemManager created\n";
    }
    
public:
    TrafficSystemManager(const TrafficSystemManager&) = delete;
    TrafficSystemManager& operator=(const TrafficSystemManager&) = delete;
    
    static TrafficSystemManager* getInstance() {
        if (instance == nullptr) {
            instance = new TrafficSystemManager();
        }
        return instance;
    }
    
    void initialize(Junction* junction, VehicleManager* vehicles) {
        mainJunction = junction;
        vehicleManager = vehicles;
        std::cout << "[SINGLETON] System initialized\n";
    }
    
    Junction* getJunction() const { return mainJunction; }
    VehicleManager* getVehicleManager() const { return vehicleManager; }
    
    void startSimulation() {
        isRunning = true;
        std::cout << "[SYSTEM] Simulation started\n";
    }
    
    void stopSimulation() {
        isRunning = false;
        std::cout << "[SYSTEM] Simulation stopped\n";
    }
    
    bool getIsRunning() const { return isRunning; }
    
    void updateSimulationTime(float dt) {
        simulationTime += dt;
    }
    
    float getSimulationTime() const { return simulationTime; }
    
    ~TrafficSystemManager() {
        std::cout << "[SINGLETON] TrafficSystemManager destroyed\n";
    }
};

TrafficSystemManager* TrafficSystemManager::instance = nullptr;
