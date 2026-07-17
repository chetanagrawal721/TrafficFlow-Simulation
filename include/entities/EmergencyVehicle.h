#pragma once

#include "Vehicle.h"
#include "utils/Constants.h"

class EmergencyVehicle : public Vehicle {
public:
    EmergencyVehicle(const Vector2D& startPos, VehicleType t, 
                     const sf::Color& col, int lane, int pri)
        : Vehicle(startPos, t, col, Constants::EMERGENCY_SPEED, lane) {
        isEmergency = true;
        priority = pri;  // 1=Ambulance, 2=FireTruck, 3=Police, 4=VIP
    }
};
