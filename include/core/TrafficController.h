#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include "core/TrafficLight.h"

class TrafficController {
private:
    int currentGreenLane;
    int savedGreenLane;
    float timer;
    float savedTimer;
    bool emergencyMode;
    bool autoMode;
    int emergencyLane;

    
    float baseGreenDuration;
    float maxGreenDuration;
    float perVehicleExtra;
    int vehicleThreshold;
    int laneCounts[4];
    float lastDynamicGreen;

public:
    TrafficController();

    void update(float dt, std::vector<TrafficLight*>& lights);
    void triggerEmergency(int lane);
    void releaseEmergency();

    int getCurrentGreenLane() const { return currentGreenLane; }
    float getTimer() const { return timer; }
    int getEmergencyLane() const { return emergencyLane; }
    bool isEmergencyActive() const { return emergencyMode; }

    void updateLaneCounts(const int counts[4]) {
        for (int i = 0; i < 4; ++i) {
            laneCounts[i] = counts[i];
        }
    }

    void setCurrentLane(int lane) { currentGreenLane = lane; }
    void setTimer(float t) { timer = t; }

    float getLastDynamicGreen() const { return lastDynamicGreen; }
};
