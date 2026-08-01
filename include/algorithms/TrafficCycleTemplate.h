#pragma once

#include <iostream>

// Abstract template
class TrafficCycleTemplate {
public:
    virtual ~TrafficCycleTemplate() = default;
    
    final void executeCycle(float deltaTime) {
        checkEmergency();
        updateLaneQueues();
        calculateGreenTiming();
        applySignalChanges();
        notifyObservers();
        updateStatistics();
    }
    
protected:
    virtual void checkEmergency() = 0;
    virtual void updateLaneQueues() = 0;
    virtual void calculateGreenTiming() = 0;
    virtual void applySignalChanges() = 0;
    virtual void notifyObservers() = 0;
    virtual void updateStatistics() = 0;
};

// Concrete implementation
class StandardTrafficCycle : public TrafficCycleTemplate {
protected:
    void checkEmergency() override {
        std::cout << "[CYCLE] Checking for emergencies...\n";
    }
    
    void updateLaneQueues() override {
        std::cout << "[CYCLE] Updating lane queue sizes...\n";
    }
    
    void calculateGreenTiming() override {
        std::cout << "[CYCLE] Calculating adaptive green timing...\n";
    }
    
    void applySignalChanges() override {
        std::cout << "[CYCLE] Applying signal changes...\n";
    }
    
    void notifyObservers() override {
        std::cout << "[CYCLE] Notifying observers...\n";
    }
    
    void updateStatistics() override {
        std::cout << "[CYCLE] Updating statistics...\n";
    }
};
