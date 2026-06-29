#pragma once

#include "Vehicle.h"

class NormalVehicle : public Vehicle {
public:
    NormalVehicle(const Vector2D& startPos, int lane)
        : Vehicle(startPos, VehicleType::NormalCar, 
                  Constants::NORMAL_VEHICLE_COLOR, 
                  Constants::NORMAL_SPEED, lane) {
        isEmergency = false;
        priority = 5;
    }
};
