#pragma once

#include <vector>
#include "core/TrafficLight.h"

// Abstract strategy
class TrafficStrategy {
public:
    virtual ~TrafficStrategy() = default;
    
    virtual float calculateGreenTime(int laneID, const int laneCounts[4]) = 0;
    virtual std::string getStrategyName() const = 0;
};

// Adaptive Strategy (Current implementation)
class AdaptiveStrategy : public TrafficStrategy {
public:
    float calculateGreenTime(int laneID, const int laneCounts[4]) override {
        float dynamicGreen = 10.0f;
        int queueLen = laneCounts[laneID];
        
        if (queueLen > 10) {
            int extraVehicles = queueLen - 10;
            float extraTime = extraVehicles * 0.2f;
            dynamicGreen = std::min(10.0f + extraTime, 20.0f);
        }
        return dynamicGreen;
    }
    
    std::string getStrategyName() const override { return "ADAPTIVE"; }
};

// Fixed Strategy (Traditional)
class FixedStrategy : public TrafficStrategy {
public:
    float calculateGreenTime(int laneID, const int laneCounts[4]) override {
        return 10.0f;
    }
    
    std::string getStrategyName() const override { return "FIXED"; }
};

// Priority Strategy
class PriorityStrategy : public TrafficStrategy {
private:
    int emergencyLane = -1;
    int emergencyPriority = 0;
    
public:
    void setEmergency(int lane, int priority) {
        emergencyLane = lane;
        emergencyPriority = priority;
    }
    
    float calculateGreenTime(int laneID, const int laneCounts[4]) override {
        if (emergencyLane == laneID && emergencyPriority < 5) {
            return 20.0f;
        }
        
        float dynamicGreen = 10.0f;
        int queueLen = laneCounts[laneID];
        
        if (queueLen > 10) {
            int extraVehicles = queueLen - 10;
            float extraTime = extraVehicles * 0.2f;
            dynamicGreen = std::min(10.0f + extraTime, 20.0f);
        }
        return dynamicGreen;
    }
    
    std::string getStrategyName() const override { return "PRIORITY"; }
};

// Eco-Friendly Strategy
class EcoStrategy : public TrafficStrategy {
public:
    float calculateGreenTime(int laneID, const int laneCounts[4]) override {
        float dynamicGreen = 10.0f;
        int queueLen = laneCounts[laneID];
        
        if (queueLen > 5) {
            int extraVehicles = queueLen - 5;
            float extraTime = extraVehicles * 0.25f;
            dynamicGreen = std::min(10.0f + extraTime, 25.0f);
        }
        return dynamicGreen;
    }
    
    std::string getStrategyName() const override { return "ECO-FRIENDLY"; }
};

// Fair Strategy
class FairStrategy : public TrafficStrategy {
public:
    float calculateGreenTime(int laneID, const int laneCounts[4]) override {
        return 12.0f;
    }
    
    std::string getStrategyName() const override { return "FAIR"; }
};
