#pragma once

#include "patterns/Observer.h"
#include <iostream>
#include <iomanip>

class TrafficMonitor : public TrafficObserver {
private:
    int totalEventsLogged = 0;
    int emergenciesLogged = 0;
    int congestionAlertsLogged = 0;
    
public:
    void onLaneStatusChanged(int laneID, float greenTime, int queueSize) override {
        totalEventsLogged++;
        std::cout << "[MONITOR] Lane " << laneID << " Updated: "
                  << "Green=" << std::fixed << std::setprecision(1) << greenTime << "s, "
                  << "Queue=" << queueSize << "\n";
    }
    
    void onEmergencyDetected(int laneID, int priority) override {
        emergenciesLogged++;
        std::cout << "[MONITOR] 🚨 EMERGENCY Lane " << laneID 
                  << " Priority=" << priority << "\n";
    }
    
    void onCongestionAlert(int laneID, int vehicleCount) override {
        congestionAlertsLogged++;
        std::cout << "[MONITOR] ⚠️  CONGESTION Lane " << laneID 
                  << " Vehicles=" << vehicleCount << "\n";
    }
    
    void onSystemStateChange(const std::string& state) override {
        std::cout << "[MONITOR] System: " << state << "\n";
    }
    
    void printMonitorReport() {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                  MONITOR REPORT                            ║\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ Total Events: " << std::setw(45) << totalEventsLogged << "║\n";
        std::cout << "║ Emergencies: " << std::setw(46) << emergenciesLogged << "║\n";
        std::cout << "║ Congestion Alerts: " << std::setw(40) << congestionAlertsLogged << "║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    }
};
