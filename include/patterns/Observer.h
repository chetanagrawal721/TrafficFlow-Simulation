#pragma once

#include <vector>
#include <iostream>
#include <memory>

// Observer interface
class TrafficObserver {
public:
    virtual ~TrafficObserver() = default;
    
    virtual void onLaneStatusChanged(int laneID, float greenTime, int queueSize) = 0;
    virtual void onEmergencyDetected(int laneID, int priority) = 0;
    virtual void onCongestionAlert(int laneID, int vehicleCount) = 0;
    virtual void onSystemStateChange(const std::string& state) = 0;
};

// Subject (Observable)
class TrafficSubject {
protected:
    std::vector<std::shared_ptr<TrafficObserver>> observers;
    
public:
    virtual ~TrafficSubject() = default;
    
    void attachObserver(std::shared_ptr<TrafficObserver> observer) {
        observers.push_back(observer);
        std::cout << "[OBSERVER] New observer attached\n";
    }
    
    void detachObserver(std::shared_ptr<TrafficObserver> observer) {
        std::cout << "[OBSERVER] Observer detached\n";
    }
    
protected:
    void notifyObserversLaneChange(int laneID, float greenTime, int queueSize) {
        for (auto& observer : observers) {
            observer->onLaneStatusChanged(laneID, greenTime, queueSize);
        }
    }
    
    void notifyObserversEmergency(int laneID, int priority) {
        for (auto& observer : observers) {
            observer->onEmergencyDetected(laneID, priority);
        }
    }
    
    void notifyObserversCongestion(int laneID, int vehicleCount) {
        for (auto& observer : observers) {
            observer->onCongestionAlert(laneID, vehicleCount);
        }
    }
    
    void notifyObserversState(const std::string& state) {
        for (auto& observer : observers) {
            observer->onSystemStateChange(state);
        }
    }
};
