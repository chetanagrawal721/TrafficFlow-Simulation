#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "utils/Constants.h"

struct SimulationState {
    // Simulation settings
    bool autoSpawn;
    int activeGreenLane;
    float simulationTime;
    
    // Vehicle data
    struct VehicleData {
        float x, y;
        int laneID;
        bool isEmergency;
        int vehicleType;  // 0=Normal, 1=Ambulance, 2=FireTruck, etc.
    };
    
    std::vector<VehicleData> vehicles;
    
    // Save to file
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        file << "# Traffic Simulation Save State\n";
        file << "autoSpawn=" << autoSpawn << "\n";
        file << "activeGreenLane=" << activeGreenLane << "\n";
        file << "simulationTime=" << simulationTime << "\n";
        file << "vehicleCount=" << vehicles.size() << "\n";
        
        file << "\n# Vehicle Data (x,y,laneID,isEmergency,type)\n";
        for (const auto& v : vehicles) {
            file << v.x << "," << v.y << "," << v.laneID << ","
                 << v.isEmergency << "," << v.vehicleType << "\n";
        }
        
        file.close();
        return true;
    }
    
    // Load from file
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        vehicles.clear();
        std::string line;
        
        while (std::getline(file, line)) {
            // Skip comments and empty lines
            if (line.empty() || line[0] == '#') continue;
            
            // Parse key=value pairs
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                
                if (key == "autoSpawn") {
                    autoSpawn = (value == "1");
                }
                else if (key == "activeGreenLane") {
                    activeGreenLane = std::stoi(value);
                }
                else if (key == "simulationTime") {
                    simulationTime = std::stof(value);
                }
            }
            // Parse vehicle data
            else if (line.find(',') != std::string::npos) {
                std::stringstream ss(line);
                VehicleData v;
                char comma;
                
                ss >> v.x >> comma >> v.y >> comma >> v.laneID >> comma
                   >> v.isEmergency >> comma >> v.vehicleType;
                
                vehicles.push_back(v);
            }
        }
        
        file.close();
        return true;
    }
};
