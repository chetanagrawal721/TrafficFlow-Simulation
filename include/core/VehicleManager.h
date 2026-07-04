#pragma once

#include <vector>
#include <queue>
#include <iostream>

#include "entities/Vehicle.h"
#include "entities/NormalVehicle.h"
#include "entities/EmergencyVehicle.h"
#include "utils/Vector2D.h"
#include "utils/Constants.h"

struct EmergencyRequest {
    EmergencyVehicle* vehicle;
    int laneID;
    float timestamp;
    int priority;
    bool processed = false;  

    bool operator<(const EmergencyRequest& other) const {
        if (priority != other.priority) {
            return priority > other.priority;
        }
        return timestamp > other.timestamp;
    }
};

class VehicleManager {
private:
    std::vector<Vehicle*> vehicles;
    std::priority_queue<EmergencyRequest> emergencyQueue;
    std::vector<EmergencyRequest> activeEmergencies;  

    int nextLaneAmbulance = 0;
    int nextLaneFireTruck = 0;
    int nextLanePolice = 0;
    int nextLaneVIP = 0;

    float spawnTimer = 0.f;
    int emergencyCount = 0;

    int getNextLaneSequence(int& currentLane) {
        int lanes[] = {0, 2, 1, 3};
        int result = lanes[currentLane % 4];
        currentLane = (currentLane + 1) % 4;
        return result;
    }

    Vector2D getSpawnPositionForLane(int lane) {
        switch(lane) {
            case 0: return Vector2D(50.f, Constants::JUNCTION_CENTER_Y - 10.f);
            case 1: return Vector2D(Constants::JUNCTION_CENTER_X - 10.f, 50.f);
            case 2: return Vector2D(Constants::WINDOW_WIDTH - 50.f, Constants::JUNCTION_CENTER_Y + 10.f);
            case 3: return Vector2D(Constants::JUNCTION_CENTER_X + 10.f, Constants::WINDOW_HEIGHT - 50.f);
            default: return Vector2D(50.f, Constants::JUNCTION_CENTER_Y - 10.f);
        }
    }

public:
    VehicleManager() {}

    ~VehicleManager() {
        clearAllVehicles();
    }

    void addNormalVehicle(int lane) {
        try {
            Vector2D spawnPos = getSpawnPositionForLane(lane);
            auto* vehicle = new NormalVehicle(spawnPos, lane);
            vehicles.push_back(vehicle);
            std::cout << "[SPAWN] Normal Vehicle spawned on Lane " << lane << std::endl;
        } catch (const std::exception& e) {
            std::cout << "[ERROR] Failed to spawn normal vehicle: " << e.what() << std::endl;
        }
    }

    void addEmergencyVehicle(VehicleType type) {
        try {
            int lane = 0;
            int& lastLane = (type == VehicleType::Ambulance) ? nextLaneAmbulance :
                            (type == VehicleType::FireTruck) ? nextLaneFireTruck :
                            (type == VehicleType::Police) ? nextLanePolice : nextLaneVIP;
            lane = getNextLaneSequence(lastLane);

            sf::Color color;
            int priority = 0;
            std::string typeName;

            if (type == VehicleType::Ambulance) {
                color = sf::Color::White;
                priority = 1;
                typeName = "AMBULANCE";
            } else if (type == VehicleType::FireTruck) {
                color = sf::Color::Red;
                priority = 2;
                typeName = "FIRETRUCK";
            } else if (type == VehicleType::Police) {
                color = sf::Color::Blue;
                priority = 3;
                typeName = "POLICE";
            } else {
                color = sf::Color::Black;
                priority = 4;
                typeName = "VIP";
            }

            Vector2D spawnPos = getSpawnPositionForLane(lane);
            auto* emergency = new EmergencyVehicle(spawnPos, type, color, lane, priority);
            vehicles.push_back(emergency);

            EmergencyRequest request;
            request.vehicle = emergency;
            request.laneID = lane;
            request.timestamp = spawnTimer;
            request.priority = priority;
            request.processed = false;  
            emergencyQueue.push(request);
            activeEmergencies.push_back(request);  
            emergencyCount++;

            std::cout << "[SPAWN] " << typeName << " Priority " << priority 
                      << " spawned on Lane " << lane << std::endl;
            
            
            std::cout << "[EMERGENCY READY] " << typeName << " is ready for override!\n";
            
        } catch (const std::exception& e) {
            std::cout << "[ERROR] Failed to spawn emergency vehicle: " << e.what() << std::endl;
        }
    }

    void update(float deltaTime) {
        spawnTimer += deltaTime;

        for (int i = static_cast<int>(vehicles.size()) - 1; i >= 0; --i) {
            auto* v = vehicles[i];
            auto pos = v->getPosition();

            bool offScreen = (pos.getX() < -100.f || pos.getX() > Constants::WINDOW_WIDTH + 100.f ||
                              pos.getY() < -100.f || pos.getY() > Constants::WINDOW_HEIGHT + 100.f);

            if (offScreen) {
                if (v->getIsEmergency()) {
                    emergencyCount--;
                    
                    for (int j = static_cast<int>(activeEmergencies.size()) - 1; j >= 0; --j) {
                        if (activeEmergencies[j].vehicle == v) {
                            activeEmergencies.erase(activeEmergencies.begin() + j);
                            break;
                        }
                    }
                }
                delete v;
                vehicles.erase(vehicles.begin() + i);
            }
        }

        for (auto* v : vehicles) {
            if (v != nullptr) {
                v->update(deltaTime);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        for (auto* v : vehicles) {
            if (v != nullptr) {
                v->draw(window);
            }
        }
    }

    void clearAllVehicles() {
        for (auto* v : vehicles) {
            if (v != nullptr) {
                delete v;
            }
        }
        vehicles.clear();
        emergencyCount = 0;
        activeEmergencies.clear();  

        while (!emergencyQueue.empty()) {
            emergencyQueue.pop();
        }
    }

    int getVehicleCount() const { return static_cast<int>(vehicles.size()); }
    int getEmergencyCount() const { return emergencyCount; }
    const std::vector<Vehicle*>& getAllVehicles() const { return vehicles; }
    
    
    bool hasUnprocessedEmergency() const {
        return !activeEmergencies.empty();
    }
    
    
    EmergencyRequest getNextUnprocessedEmergency() {
        if (!activeEmergencies.empty()) {
            EmergencyRequest req = activeEmergencies[0];
            activeEmergencies.erase(activeEmergencies.begin());
            return req;
        }
        return EmergencyRequest{nullptr, -1, 0.f, 0, true};
    }

    
    bool hasEmergencyRequest() const { 
        return !emergencyQueue.empty(); 
    }

    EmergencyRequest getNextEmergencyRequest() {
        if (emergencyQueue.empty()) {
            return EmergencyRequest{nullptr, -1, 0.f, 0, true};
        }
        EmergencyRequest request = emergencyQueue.top();
        emergencyQueue.pop();
        return request;
    }

    int getLaneVehicleCount(int lane) const {
        int count = 0;
        for (auto* v : vehicles) {
            if (!v) continue;
            if (v->getIsEmergency()) continue;
            if (v->getLaneID() == lane) {
                count++;
            }
        }
        return count;
    }
};
