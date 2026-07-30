#pragma once

#include <iostream>
#include <string>

class TrafficSystem;

// Abstract State
class TrafficState {
public:
    virtual ~TrafficState() = default;
    
    virtual void enter(TrafficSystem* system) = 0;
    virtual void exit(TrafficSystem* system) = 0;
    virtual void handleEmergency(TrafficSystem* system, int lane) = 0;
    virtual std::string getName() const = 0;
};

// Normal Operation State
class NormalState : public TrafficState {
public:
    void enter(TrafficSystem* system) override {
        std::cout << "[STATE] Entering NORMAL operation\n";
    }
    
    void exit(TrafficSystem* system) override {
        std::cout << "[STATE] Exiting NORMAL operation\n";
    }
    
    void handleEmergency(TrafficSystem* system, int lane) override {
        std::cout << "[STATE] Emergency detected! Switching to EMERGENCY state\n";
    }
    
    std::string getName() const override { return "NORMAL"; }
};

// Emergency State
class EmergencyState : public TrafficState {
private:
    int emergencyLane = -1;
    float emergencyTimer = 0.0f;
    
public:
    void enter(TrafficSystem* system) override {
        std::cout << "[STATE] Entering EMERGENCY operation\n";
        emergencyTimer = 0.0f;
    }
    
    void exit(TrafficSystem* system) override {
        std::cout << "[STATE] Exiting EMERGENCY operation\n";
    }
    
    void handleEmergency(TrafficSystem* system, int lane) override {
        emergencyLane = lane;
        std::cout << "[STATE] Emergency on Lane " << lane << "\n";
    }
    
    std::string getName() const override { return "EMERGENCY"; }
    
    int getEmergencyLane() const { return emergencyLane; }
    void updateTimer(float dt) { emergencyTimer += dt; }
    float getEmergencyTimer() const { return emergencyTimer; }
};

// Maintenance State
class MaintenanceState : public TrafficState {
public:
    void enter(TrafficSystem* system) override {
        std::cout << "[STATE] Entering MAINTENANCE mode - All lights RED\n";
    }
    
    void exit(TrafficSystem* system) override {
        std::cout << "[STATE] Exiting MAINTENANCE mode\n";
    }
    
    void handleEmergency(TrafficSystem* system, int lane) override {
        std::cout << "[STATE] Emergency during maintenance - ignored\n";
    }
    
    std::string getName() const override { return "MAINTENANCE"; }
};

// Congestion State
class CongestionState : public TrafficState {
private:
    int congestionLevel = 0;
    
public:
    void enter(TrafficSystem* system) override {
        std::cout << "[STATE] Entering CONGESTION management\n";
        congestionLevel = 1;
    }
    
    void exit(TrafficSystem* system) override {
        std::cout << "[STATE] Exiting CONGESTION management\n";
        congestionLevel = 0;
    }
    
    void handleEmergency(TrafficSystem* system, int lane) override {
        std::cout << "[STATE] Emergency during congestion - prioritizing emergency\n";
    }
    
    std::string getName() const override { return "CONGESTION"; }
    
    void incrementCongestion() { 
        congestionLevel = std::min(congestionLevel + 1, 5);
    }
    
    int getCongestionLevel() const { return congestionLevel; }
};
