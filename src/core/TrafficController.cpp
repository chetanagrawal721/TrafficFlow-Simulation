#include "core/TrafficController.h"
#include "utils/Constants.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

TrafficController::TrafficController()
    : currentGreenLane(0),
      savedGreenLane(0),
      timer(0.f),
      savedTimer(0.f),
      emergencyMode(false),
      autoMode(true),
      emergencyLane(-1),
      lastDynamicGreen(10.0f) {

    baseGreenDuration = Constants::BASE_GREEN_DURATION;
    maxGreenDuration = Constants::MAX_GREEN_DURATION;
    perVehicleExtra = Constants::PER_VEHICLE_EXTRA_TIME;
    vehicleThreshold = Constants::VEHICLE_QUEUE_THRESHOLD;

    for (int i = 0; i < 4; ++i) {
        laneCounts[i] = 0;
    }
}

void TrafficController::update(float dt, std::vector<TrafficLight*>& lights) {
    try {
        if (lights.empty() || lights.size() < 4) {
            return;
        }

        if (emergencyMode) {
            return;
        }

        if (!autoMode) {
            return;
        }

        timer += dt;

        LightState currentState = lights[currentGreenLane]->getState();

        // Calculate dynamic green with detailed logging
        float dynamicGreen = baseGreenDuration;
        int queueLen = laneCounts[currentGreenLane];

        if (queueLen > vehicleThreshold) {
            int extraVehicles = queueLen - vehicleThreshold;
            float extraTime = perVehicleExtra * static_cast<float>(extraVehicles);
            dynamicGreen = std::min(baseGreenDuration + extraTime, maxGreenDuration);
        }

        lastDynamicGreen = dynamicGreen;

        // GREEN → YELLOW
        if (currentState == LightState::Green && timer >= dynamicGreen) {
            std::cout << "\n[TIMING] Lane " << currentGreenLane << " GREEN Duration Calculation:\n";
            std::cout << "  Queue Size: " << queueLen << " vehicles\n";
            std::cout << "  Base Green Time: " << std::fixed << std::setprecision(1) 
                      << baseGreenDuration << "s\n";
            
            if (queueLen > vehicleThreshold) {
                int extra = queueLen - vehicleThreshold;
                std::cout << "  Extra Vehicles: " << extra << " (above threshold of " 
                          << vehicleThreshold << ")\n";
                std::cout << "  Extra Time Added: " << std::fixed << std::setprecision(2) 
                          << (perVehicleExtra * extra) << "s (" << extra << " × " 
                          << perVehicleExtra << "s/vehicle)\n";
            }
            
            std::cout << "  FINAL GREEN TIME: " << std::fixed << std::setprecision(2) 
                      << dynamicGreen << "s\n";
            std::cout << "  Switching to YELLOW...\n";

            lights[currentGreenLane]->setState(LightState::Yellow);
            timer = 0.f;
        }
        // YELLOW → RED + next lane GREEN
        else if (currentState == LightState::Yellow && timer >= Constants::YELLOW_DURATION) {
            lights[currentGreenLane]->setState(LightState::Red);

            int nextLane = (currentGreenLane + 1) % 4;
            lights[nextLane]->setState(LightState::Green);
            
            std::string laneNames[] = {"WEST", "NORTH", "EAST", "SOUTH"};
            std::cout << "\n[CYCLE] Lane " << currentGreenLane << " (" << laneNames[currentGreenLane] 
                      << ") → Red\n";
            std::cout << "[CYCLE] Lane " << nextLane << " (" << laneNames[nextLane] 
                      << ") → Green\n";

            currentGreenLane = nextLane;
            timer = 0.f;
        }
    } catch (const std::exception& e) {
        std::cout << "[ERROR] TrafficController::update() - " << e.what() << std::endl;
    }
}

void TrafficController::triggerEmergency(int lane) {
    try {
        emergencyMode = true;
        emergencyLane = lane;
        savedGreenLane = currentGreenLane;
        savedTimer = timer;
        std::cout << "[EMERGENCY] Override activated for Lane " << lane << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[ERROR] TrafficController::triggerEmergency() - " << e.what() << std::endl;
    }
}

void TrafficController::releaseEmergency() {
    try {
        emergencyMode = false;
        currentGreenLane = savedGreenLane;
        timer = savedTimer;
        autoMode = true;
        emergencyLane = -1;
        std::cout << "[EMERGENCY] Released, resuming Lane " << currentGreenLane << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[ERROR] TrafficController::releaseEmergency() - " << e.what() << std::endl;
    }
}
