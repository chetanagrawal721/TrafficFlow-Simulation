#pragma once

#include <SFML/Graphics.hpp>
#include "utils/Vector2D.h"
#include "utils/Constants.h"

enum class VehicleType {
    NormalCar = 0,
    Ambulance = 1,
    FireTruck = 2,
    Police = 3,
    VIP = 4
};

class Vehicle {
protected:
    Vector2D position;
    VehicleType type;
    sf::Color color;
    float speed;
    int laneID;
    bool canMove;
    bool isEmergency;
    int priority;
    sf::CircleShape shape;
    float emergencyFlashTimer;
    
public:
    Vehicle(const Vector2D& pos, VehicleType t, const sf::Color& col, 
            float spd, int lane)
        : position(pos), type(t), color(col), speed(spd), laneID(lane),
          canMove(true), isEmergency(false), priority(5), emergencyFlashTimer(0.f) {
        shape.setRadius(Constants::VEHICLE_WIDTH / 2.f);
        shape.setFillColor(color);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);
    }
    
    virtual ~Vehicle() = default;
    
    virtual void update(float deltaTime) {
        if (!canMove) return; 
        
        float moveDistance = speed * deltaTime;
        
        if (laneID == 0) {
            position.setX(position.getX() + moveDistance);
        }
        else if (laneID == 1) {
            position.setY(position.getY() + moveDistance);
        }
        else if (laneID == 2) {
            position.setX(position.getX() - moveDistance);
        }
        else if (laneID == 3) {
            position.setY(position.getY() - moveDistance);
        }
        
       
        if (isEmergency) {
            emergencyFlashTimer += deltaTime;
            if (emergencyFlashTimer >= 0.5f) {
                emergencyFlashTimer = 0.f;
            }
        }
    }
    
    virtual void draw(sf::RenderWindow& window) {
        shape.setPosition({position.getX() - shape.getRadius(),
                          position.getY() - shape.getRadius()});
        
        
        if (isEmergency && emergencyFlashTimer > 0.25f) {
            sf::CircleShape flashShape = shape;
            flashShape.setFillColor(sf::Color(color.r/2, color.g/2, color.b/2, 128));
            window.draw(flashShape);
        } else {
            window.draw(shape);
        }
        
       
        if (!canMove && !isEmergency) {
            sf::CircleShape stoppedIndicator = shape;
            stoppedIndicator.setOutlineColor(sf::Color::Red);
            stoppedIndicator.setOutlineThickness(3.f);
            window.draw(stoppedIndicator);
        }
    }
    
    void checkTrafficLight(const class TrafficLight& light);
    
    void applyForce(const Vector2D& /* force */) {}
    
   
    Vector2D getPosition() const { return position; }
    VehicleType getType() const { return type; }
    float getSpeed() const { return speed; }
    int getLaneID() const { return laneID; }
    bool getCanMove() const { return canMove; }
    bool getIsEmergency() const { return isEmergency; }
    int getPriority() const { return priority; }
    
    
    void setPosition(const Vector2D& pos) { position = pos; }
    void setCanMove(bool move) { canMove = move; }
};
