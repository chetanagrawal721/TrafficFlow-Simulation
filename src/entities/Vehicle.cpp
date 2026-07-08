#include "entities/Vehicle.h"
#include "core/TrafficLight.h"
#include "utils/Constants.h"
#include <cmath>

void Vehicle::checkTrafficLight(const TrafficLight& light) {
    // ✅ Emergency vehicles ALWAYS move at full speed, ignoring lights
    if (isEmergency) {
        canMove = true;
        return;
    }
    
    // ✅ Normal vehicles check light
    Vector2D lightPos = light.getPosition();
    float distanceToLight = position.distanceTo(lightPos);  // ✅ Fixed: vpos -> position
    
    // ✅ Only check if within stopping distance (50 pixels)
    if (distanceToLight > Constants::STOPPING_DISTANCE) {
        canMove = true;
        return;
    }
    
    // Check if vehicle is approaching the light
    bool isApproaching = false;
    if (laneID == 0 && position.getX() < lightPos.getX()) isApproaching = true;
    else if (laneID == 1 && position.getY() < lightPos.getY()) isApproaching = true;
    else if (laneID == 2 && position.getX() > lightPos.getX()) isApproaching = true;
    else if (laneID == 3 && position.getY() > lightPos.getY()) isApproaching = true;
    
    if (!isApproaching) {
        canMove = true;
        return;
    }
    
    // ✅ Check light state and STOP at RED
    if (light.getState() == LightState::Red) {
        canMove = false;  // STOP
    } else if (light.getState() == LightState::Yellow) {
        // Vehicle already in motion continues, stopped vehicles remain stopped
        if (!canMove) {
            canMove = false;  // Stay stopped if already stopped
        }
    } else if (light.getState() == LightState::Green) {
        canMove = true;  // GO
    }
}
