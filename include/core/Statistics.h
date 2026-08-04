#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <chrono>

class Statistics {
private:
    // ╔════════════════════════════════════════════════════════╗
    // ║                 VEHICLE TRACKING                       ║
    // ╚════════════════════════════════════════════════════════╝
    int totalVehiclesSpawned{0};
    int totalVehiclesPassed{0};
    int currentVehicleCount{0};

    // Per-lane tracking
    int vehiclesPassed[4] = {0, 0, 0, 0};
    int vehiclesQueued[4] = {0, 0, 0, 0};
    float totalGreenTime[4] = {0.f, 0.f, 0.f, 0.f};
    float dynamicGreenUsed[4] = {0.f, 0.f, 0.f, 0.f};
    int cycleCount[4] = {0, 0, 0, 0};

    // ╔════════════════════════════════════════════════════════╗
    // ║              EMERGENCY TRACKING                        ║
    // ╚════════════════════════════════════════════════════════╝
    int totalEmergenciesHandled{0};
    int emergenciesInQueue{0};
    float averageEmergencyResponseTime{0.f};
    std::vector<float> emergencyResponseTimes;

    // Emergency breakdown by type
    int ambulancesHandled{0};
    int firetrucksHandled{0};
    int policeHandled{0};
    int vipsHandled{0};

    // ╔════════════════════════════════════════════════════════╗
    // ║            TRAFFIC LIGHT PERFORMANCE                   ║
    // ╚════════════════════════════════════════════════════════╝
    std::vector<float> waitTimes;
    float averageWaitTime{0.f};
    float maxWaitTime{0.f};
    float minWaitTime{999999.f};

    // ╔════════════════════════════════════════════════════════╗
    // ║          ADAPTIVE TIMING TRACKING                      ║
    // ╚════════════════════════════════════════════════════════╝
    float baseGreenDuration{10.0f};
    float maxGreenDuration{20.0f};
    float totalAdaptiveGreenExtension{0.f};
    int timingsAdjusted{0};

    // ╔════════════════════════════════════════════════════════╗
    // ║             SIMULATION PERFORMANCE                     ║
    // ╚════════════════════════════════════════════════════════╝
    float totalSimulationTime{0.f};
    std::vector<float> fpsHistory;
    float averageFPS{0.f};

    // ╔════════════════════════════════════════════════════════╗
    // ║                    TIMING DATA                         ║
    // ╚════════════════════════════════════════════════════════╝
    std::chrono::steady_clock::time_point startTime;

public:
    Statistics() {
        startTime = std::chrono::steady_clock::now();
    }

    // ╔════════════════════════════════════════════════════════╗
    // ║              VEHICLE STATISTICS METHODS                ║
    // ╚════════════════════════════════════════════════════════╝
    
    void recordVehicleSpawned() {
        totalVehiclesSpawned++;
        currentVehicleCount++;
    }

    void recordVehiclePassed(int laneID) {
        totalVehiclesPassed++;
        currentVehicleCount--;
        if (laneID >= 0 && laneID < 4) {
            vehiclesPassed[laneID]++;
        }
    }

    void recordVehicleRemoved() {
        currentVehicleCount--;
    }

    void recordQueueSize(int laneID, int queueSize) {
        if (laneID >= 0 && laneID < 4) {
            vehiclesQueued[laneID] = queueSize;
        }
    }

    // ╔════════════════════════════════════════════════════════╗
    // ║           ADAPTIVE TIMING STATISTICS                   ║
    // ╚════════════════════════════════════════════════════════╝
    
    void recordGreenTiming(int laneID, float baseTiming, float dynamicTiming) {
        if (laneID >= 0 && laneID < 4) {
            totalGreenTime[laneID] += dynamicTiming;
            dynamicGreenUsed[laneID] = dynamicTiming;
            cycleCount[laneID]++;

            // Track adaptive extension
            if (dynamicTiming > baseTiming) {
                totalAdaptiveGreenExtension += (dynamicTiming - baseTiming);
                timingsAdjusted++;
            }
        }
    }

    // ╔════════════════════════════════════════════════════════╗
    // ║            EMERGENCY STATISTICS METHODS                ║
    // ╚════════════════════════════════════════════════════════╝
    
    void recordEmergencyStarted() {
        emergenciesInQueue++;
    }

    void recordEmergencyCompleted(float responseTime, int emergencyType) {
        totalEmergenciesHandled++;
        emergenciesInQueue--;
        emergencyResponseTimes.push_back(responseTime);

        // Track by type
        switch(emergencyType) {
            case 1: ambulancesHandled++; break;
            case 2: firetrucksHandled++; break;
            case 3: policeHandled++; break;
            case 4: vipsHandled++; break;
        }

        // Calculate average
        float sum = 0.f;
        for (float time : emergencyResponseTimes) {
            sum += time;
        }
        averageEmergencyResponseTime = sum / emergencyResponseTimes.size();
    }

    // ╔════════════════════════════════════════════════════════╗
    // ║              WAIT TIME TRACKING                        ║
    // ╚════════════════════════════════════════════════════════╝
    
    void recordVehicleWaitTime(float waitTime) {
        waitTimes.push_back(waitTime);
        if (waitTime > maxWaitTime) {
            maxWaitTime = waitTime;
        }
        if (waitTime < minWaitTime && waitTime > 0.f) {
            minWaitTime = waitTime;
        }

        // Calculate average
        float sum = 0.f;
        for (float time : waitTimes) {
            sum += time;
        }
        averageWaitTime = sum / waitTimes.size();
    }

    // ╔════════════════════════════════════════════════════════╗
    // ║             PERFORMANCE TRACKING                       ║
    // ╚════════════════════════════════════════════════════════╝
    
    void recordFrameTime(float fps) {
        fpsHistory.push_back(fps);
        // Keep only last 60 frames
        if (fpsHistory.size() > 60) {
            fpsHistory.erase(fpsHistory.begin());
        }

        // Calculate average FPS
        float sum = 0.f;
        for (float f : fpsHistory) {
            sum += f;
        }
        averageFPS = sum / fpsHistory.size();
    }

    void updateSimulationTime(float dt) {
        totalSimulationTime += dt;
    }

    // ╔════════════════════════════════════════════════════════╗
    // ║                    GETTERS                             ║
    // ╚════════════════════════════════════════════════════════╝
    
    int getTotalVehiclesSpawned() const { return totalVehiclesSpawned; }
    int getTotalVehiclesPassed() const { return totalVehiclesPassed; }
    int getCurrentVehicleCount() const { return currentVehicleCount; }
    
    int getTotalEmergenciesHandled() const { return totalEmergenciesHandled; }
    int getEmergenciesInQueue() const { return emergenciesInQueue; }
    float getAverageEmergencyResponseTime() const { return averageEmergencyResponseTime; }
    
    float getAverageWaitTime() const { return averageWaitTime; }
    float getMaxWaitTime() const { return maxWaitTime; }
    float getMinWaitTime() const { return minWaitTime; }
    
    float getTotalSimulationTime() const { return totalSimulationTime; }
    float getAverageFPS() const { return averageFPS; }
    
    int getVehiclesPassed(int laneID) const {
        if (laneID >= 0 && laneID < 4) {
            return vehiclesPassed[laneID];
        }
        return 0;
    }

    int getVehiclesQueued(int laneID) const {
        if (laneID >= 0 && laneID < 4) {
            return vehiclesQueued[laneID];
        }
        return 0;
    }

    float getTotalGreenTime(int laneID) const {
        if (laneID >= 0 && laneID < 4) {
            return totalGreenTime[laneID];
        }
        return 0.f;
    }

    float getAvgGreenTime(int laneID) const {
        if (laneID >= 0 && laneID < 4 && cycleCount[laneID] > 0) {
            return totalGreenTime[laneID] / cycleCount[laneID];
        }
        return 0.f;
    }

    int getCycleCount(int laneID) const {
        if (laneID >= 0 && laneID < 4) {
            return cycleCount[laneID];
        }
        return 0;
    }

    // ╔════════════════════════════════════════════════════════╗
    // ║              REPORT GENERATION                         ║
    // ╚════════════════════════════════════════════════════════╝
    
    void printLaneReport(int laneID) {
        if (laneID < 0 || laneID >= 4) return;

        std::string laneNames[] = {"WEST (0)", "NORTH (1)", "EAST (2)", "SOUTH (3)"};
        
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║  LANE " << laneNames[laneID] << " - DETAILED STATISTICS\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ Vehicles Passed:        " << std::setw(3) << vehiclesPassed[laneID] << "\n";
        std::cout << "║ Current Queue Size:     " << std::setw(3) << vehiclesQueued[laneID] << "\n";
        std::cout << "║ Total Green Time:       " << std::fixed << std::setprecision(1) 
                  << std::setw(6) << totalGreenTime[laneID] << " seconds\n";
        std::cout << "║ Last Green Duration:    " << std::fixed << std::setprecision(2) 
                  << std::setw(6) << dynamicGreenUsed[laneID] << " seconds\n";
        std::cout << "║ Cycle Count:            " << std::setw(3) << cycleCount[laneID] << "\n";
        std::cout << "║ Avg Green Duration:     " << std::fixed << std::setprecision(2) 
                  << std::setw(6) << getAvgGreenTime(laneID) << " seconds\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    }

    std::string generateReport() const {
        std::stringstream ss;
        
        ss << "\n╔════════════════════════════════════════════════════════════╗\n";
        ss << "║         COMPLETE TRAFFIC SIMULATION REPORT                ║\n";
        ss << "╠════════════════════════════════════════════════════════════╣\n";
        ss << "║ SIMULATION DURATION: " << std::fixed << std::setprecision(1) 
           << std::setw(6) << totalSimulationTime << "s\n";
        ss << "║ AVERAGE FPS: " << std::fixed << std::setprecision(1) 
           << std::setw(6) << averageFPS << "\n";
        ss << "╠════════════════════════════════════════════════════════════╣\n\n";

        ss << "║ VEHICLE STATISTICS:\n";
        ss << "║   Total Spawned: " << totalVehiclesSpawned << "\n";
        ss << "║   Total Passed: " << totalVehiclesPassed << "\n";
        ss << "║   Currently Active: " << currentVehicleCount << "\n";
        ss << "║   Average Wait Time: " << std::fixed << std::setprecision(2) 
           << averageWaitTime << "s\n";
        ss << "║   Max Wait Time: " << std::fixed << std::setprecision(2) 
           << maxWaitTime << "s\n";
        ss << "║   Min Wait Time: " << std::fixed << std::setprecision(2) 
           << minWaitTime << "s\n\n";

        ss << "║ EMERGENCY STATISTICS:\n";
        ss << "║   Total Handled: " << totalEmergenciesHandled << "\n";
        ss << "║   Currently in Queue: " << emergenciesInQueue << "\n";
        ss << "║   Avg Response Time: " << std::fixed << std::setprecision(2) 
           << averageEmergencyResponseTime << "s\n";
        ss << "║   Ambulances: " << ambulancesHandled << "  |  ";
        ss << "FireTrucks: " << firetrucksHandled << "  |  ";
        ss << "Police: " << policeHandled << "  |  ";
        ss << "VIPs: " << vipsHandled << "\n\n";

        ss << "║ ADAPTIVE TIMING STATISTICS:\n";
        ss << "║   Base Green Duration: " << std::fixed << std::setprecision(1) 
           << baseGreenDuration << "s\n";
        ss << "║   Max Green Duration: " << std::fixed << std::setprecision(1) 
           << maxGreenDuration << "s\n";
        ss << "║   Total Adaptive Extensions: " << std::fixed << std::setprecision(1) 
           << totalAdaptiveGreenExtension << "s\n";
        ss << "║   Times Adjusted: " << timingsAdjusted << "\n\n";

        ss << "║ LANE-BY-LANE BREAKDOWN:\n";
        std::string lanes[] = {"WEST", "NORTH", "EAST", "SOUTH"};
        for (int i = 0; i < 4; ++i) {
            ss << "║   Lane " << i << " (" << lanes[i] << "):\n";
            ss << "║     Vehicles Passed: " << std::setw(3) << vehiclesPassed[i] << "\n";
            ss << "║     Queue Size: " << std::setw(3) << vehiclesQueued[i] << "\n";
            ss << "║     Total Green Time: " << std::fixed << std::setprecision(1) 
               << std::setw(6) << totalGreenTime[i] << "s\n";
            ss << "║     Avg Green Time: " << std::fixed << std::setprecision(2) 
               << std::setw(6) << getAvgGreenTime(i) << "s\n";
            ss << "║     Cycles: " << cycleCount[i] << "\n";
            ss << "║\n";
        }

        ss << "╚════════════════════════════════════════════════════════════╝\n";
        
        return ss.str();
    }

    void printDetailedReport() const {
        std::cout << generateReport();
    }

    // ╔════════════════════════════════════════════════════════╗
    // ║                  RESET FUNCTION                       ║
    // ╚════════════════════════════════════════════════════════╝
    
    void reset() {
        // Reset vehicle tracking
        totalVehiclesSpawned = 0;
        totalVehiclesPassed = 0;
        currentVehicleCount = 0;

        // Reset per-lane data
        for (int i = 0; i < 4; ++i) {
            vehiclesPassed[i] = 0;
            vehiclesQueued[i] = 0;
            totalGreenTime[i] = 0.f;
            dynamicGreenUsed[i] = 0.f;
            cycleCount[i] = 0;
        }

        // Reset emergency tracking
        totalEmergenciesHandled = 0;
        emergenciesInQueue = 0;
        averageEmergencyResponseTime = 0.f;
        emergencyResponseTimes.clear();
        ambulancesHandled = 0;
        firetrucksHandled = 0;
        policeHandled = 0;
        vipsHandled = 0;

        // Reset wait time tracking
        waitTimes.clear();
        averageWaitTime = 0.f;
        maxWaitTime = 0.f;
        minWaitTime = 999999.f;

        // Reset adaptive timing
        totalAdaptiveGreenExtension = 0.f;
        timingsAdjusted = 0;

        // Reset performance tracking
        totalSimulationTime = 0.f;
        fpsHistory.clear();
        averageFPS = 0.f;

        startTime = std::chrono::steady_clock::now();
    }

    // ╔════════════════════════════════════════════════════════╗
    // ║              EFFICIENCY METRICS                        ║
    // ╚════════════════════════════════════════════════════════╝
    
    float calculateThroughputPerMinute() const {
        if (totalSimulationTime <= 0.f) return 0.f;
        return (totalVehiclesPassed / totalSimulationTime) * 60.f;
    }

    float calculateAdaptiveEfficiency() const {
        if (timingsAdjusted == 0) return 0.f;
        return (totalAdaptiveGreenExtension / (baseGreenDuration * timingsAdjusted)) * 100.f;
    }

    float calculateEmergencyEfficiency() const {
        if (totalEmergenciesHandled == 0) return 0.f;
        return (totalEmergenciesHandled / static_cast<float>(totalEmergenciesHandled + emergenciesInQueue)) * 100.f;
    }
};
