#pragma once

#include "entities/Vehicle.h"
#include "utils/Vector2D.h"
#include <vector>
#include <cmath>

namespace SocialForce {
    const float BRAKE_FORCE = 0.8f;
    const float ALIGN_FORCE = 0.05f;
    
    inline Vector2D computeForce(const Vehicle& me, 
                                  const std::vector<Vehicle*>& others, 
                                  float laneY) {
        Vector2D totalForce(0.f, 0.f);
        Vector2D myPos = me.getPosition();
        int myLane = me.getLaneID();
        
        float nearestFrontDist = 999999.f;
        
        for (const auto* other : others) {
            if (other->getLaneID() != myLane) continue;
            
            Vector2D oPos = other->getPosition();
            
            
            float dist = myPos.distanceTo(oPos);
            
            
            if (oPos.getX() > myPos.getX() && dist < nearestFrontDist) {
                nearestFrontDist = dist;
            }
        }
        
        if (nearestFrontDist < 50.f) {
            float diff = (50.f - nearestFrontDist) / 50.f;
            // ✅ Simple force computation without direct x access
            totalForce = totalForce - Vector2D(diff * BRAKE_FORCE, 0.f);
        }
        
       
        float yOffset = laneY - myPos.getY();
        totalForce = totalForce + Vector2D(0.f, yOffset * ALIGN_FORCE);
        
        return totalForce;
    }
}
