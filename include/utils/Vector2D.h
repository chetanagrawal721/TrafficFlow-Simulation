#pragma once

#include <cmath>

class Vector2D {
private:
    float x, y;
    
public:
    Vector2D(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    
    float getX() const { return x; }
    float getY() const { return y; }
    
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }
    
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }
    
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }
    
    Vector2D normalized() const {
        float mag = magnitude();
        if (mag > 0.0001f) {
            return Vector2D(x / mag, y / mag);
        }
        return Vector2D(0, 0);
    }
    
    float distanceTo(const Vector2D& other) const {
        return (*this - other).magnitude();
    }
    
    static float distance(const Vector2D& a, const Vector2D& b) {
        return a.distanceTo(b);
    }
};
