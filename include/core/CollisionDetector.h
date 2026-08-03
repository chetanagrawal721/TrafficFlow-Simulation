#pragma once

#include <vector>
#include <cmath>
#include "entities/Vehicle.h"
#include "utils/Vector2D.h"

class CollisionDetector {
public:
    // Check if two vehicles are too close
    static bool checkCollision(const Vehicle* v1, const Vehicle* v2, float safeDistance = 40.f) {
        Vector2D pos1 = v1->getPosition();
        Vector2D pos2 = v2->getPosition();
        
        float distance = Vector2D::distance(pos1, pos2);
        return distance < safeDistance;
    }
    
    // Find nearest vehicle ahead in same lane
    static Vehicle* findNearestVehicleAhead(const Vehicle* current, 
                                            const std::vector<Vehicle*>& allVehicles) {
        Vehicle* nearest = nullptr;
        float minDistance = 1000000.f;
        
        Vector2D currentPos = current->getPosition();
        int currentLane = current->getLaneID();
        
        for (auto* other : allVehicles) {
            if (other == current) continue;
            if (other->getLaneID() != currentLane) continue;
            
            Vector2D otherPos = other->getPosition();
            
            // Only consider vehicles AHEAD (greater X for horizontal lanes)
            if (currentLane == 0 || currentLane == 2) {
                if (otherPos.x > currentPos.x) {
                    float distance = otherPos.x - currentPos.x;
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearest = other;
                    }
                }
            }
            // For vertical lanes (future)
            else if (currentLane == 1 || currentLane == 3) {
                if (otherPos.y > currentPos.y) {
                    float distance = otherPos.y - currentPos.y;
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearest = other;
                    }
                }
            }
        }
        
        return nearest;
    }
    
    // Calculate safe speed to avoid collision
    static float calculateSafeSpeed(const Vehicle* current, 
                                     Vehicle* ahead, 
                                     float normalSpeed) {
        if (!ahead) return normalSpeed;
        
        Vector2D currentPos = current->getPosition();
        Vector2D aheadPos = ahead->getPosition();
        
        float distance = Vector2D::distance(currentPos, aheadPos);
        const float MIN_DISTANCE = 50.f;  // Minimum safe distance
        const float BRAKE_DISTANCE = 100.f;  // Start slowing distance
        
        if (distance < MIN_DISTANCE) {
            return 0.f;  // Stop completely
        }
        else if (distance < BRAKE_DISTANCE) {
            // Gradually reduce speed as distance decreases
            float speedFactor = (distance - MIN_DISTANCE) / (BRAKE_DISTANCE - MIN_DISTANCE);
            return normalSpeed * speedFactor;
        }
        
        return normalSpeed;
    }
    
    // Prevent vehicles from overlapping during update
    static void resolveCollision(Vehicle* v1, Vehicle* v2) {
        Vector2D pos1 = v1->getPosition();
        Vector2D pos2 = v2->getPosition();
        
        float distance = Vector2D::distance(pos1, pos2);
        const float MIN_DISTANCE = 35.f;
        
        if (distance < MIN_DISTANCE && distance > 0.01f) {
            // Calculate separation vector
            Vector2D diff = pos1 - pos2;
            Vector2D normalized = diff.normalized();
            
            // Push vehicles apart slightly
            float overlap = MIN_DISTANCE - distance;
            Vector2D separation = normalized * (overlap * 0.5f);
            
            v1->setPosition(pos1 + separation);
            v2->setPosition(pos2 - separation);
        }
    }
};
