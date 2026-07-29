#pragma once

#include "strategies/TrafficStrategy.h"
#include <memory>
#include <iostream>

class StrategyController {
private:
    std::unique_ptr<TrafficStrategy> currentStrategy;
    
public:
    StrategyController() : currentStrategy(std::make_unique<AdaptiveStrategy>()) {}
    
    void switchStrategy(std::unique_ptr<TrafficStrategy> newStrategy) {
        std::cout << "[STRATEGY] Switching from " << currentStrategy->getStrategyName()
                  << " to " << newStrategy->getStrategyName() << "\n";
        currentStrategy = std::move(newStrategy);
    }
    
    float getGreenTime(int laneID, const int laneCounts[4]) {
        return currentStrategy->calculateGreenTime(laneID, laneCounts);
    }
    
    std::string getCurrentStrategy() const {
        return currentStrategy->getStrategyName();
    }
};
