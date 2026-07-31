#pragma once

#include <vector>
#include <memory>
#include <iostream>

class TrafficLight;

// Abstract Component
class TrafficComponent {
public:
    virtual ~TrafficComponent() = default;
    
    virtual void update(float deltaTime) = 0;
    virtual void display() const = 0;
    virtual int getVehicleCount() const = 0;
    virtual std::string getName() const = 0;
};

// Leaf: Single Lane
class Lane : public TrafficComponent {
private:
    int laneID;
    TrafficLight* light;
    int vehicleCount = 0;
    
public:
    Lane(int id, TrafficLight* trafficLight) 
        : laneID(id), light(trafficLight) {
        std::cout << "[LANE] Lane " << id << " created\n";
    }
    
    void update(float deltaTime) override {
        // Update single lane
    }
    
    void display() const override {
        std::cout << "  Lane " << laneID << ": " << vehicleCount << " vehicles\n";
    }
    
    int getVehicleCount() const override { return vehicleCount; }
    
    std::string getName() const override { 
        return "Lane " + std::to_string(laneID); 
    }
    
    void setVehicleCount(int count) { vehicleCount = count; }
};

// Composite: Junction
class JunctionComposite : public TrafficComponent {
private:
    std::vector<std::shared_ptr<TrafficComponent>> lanes;
    int junctionID;
    
public:
    JunctionComposite(int id) : junctionID(id) {
        std::cout << "[JUNCTION] Junction " << id << " created\n";
    }
    
    void addLane(std::shared_ptr<TrafficComponent> lane) {
        lanes.push_back(lane);
        std::cout << "[JUNCTION] Lane added to Junction " << junctionID << "\n";
    }
    
    void update(float deltaTime) override {
        for (auto& lane : lanes) {
            lane->update(deltaTime);
        }
    }
    
    void display() const override {
        std::cout << "\nJunction " << junctionID << ":\n";
        for (const auto& lane : lanes) {
            lane->display();
        }
    }
    
    int getVehicleCount() const override {
        int total = 0;
        for (const auto& lane : lanes) {
            total += lane->getVehicleCount();
        }
        return total;
    }
    
    std::string getName() const override { 
        return "Junction " + std::to_string(junctionID); 
    }
};
