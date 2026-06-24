#pragma once

#include <SFML/Graphics.hpp>

namespace Constants {
    // ╔════════════════════════════════════════════════════════╗
    // ║                    WINDOW SETTINGS                     ║
    // ╚════════════════════════════════════════════════════════╝
    const unsigned int WINDOW_WIDTH = 1200;
    const unsigned int WINDOW_HEIGHT = 900;
    const unsigned int TARGET_FPS = 60;

    // ╔════════════════════════════════════════════════════════╗
    // ║                    JUNCTION SETTINGS                   ║
    // ╚════════════════════════════════════════════════════════╝
    const float JUNCTION_CENTER_X = 600.f;
    const float JUNCTION_CENTER_Y = 450.f;
    const float JUNCTION_SIZE = 150.f;

    // ╔════════════════════════════════════════════════════════╗
    // ║                      ROAD SETTINGS                     ║
    // ╚════════════════════════════════════════════════════════╝
    const float ROAD_WIDTH = 80.f;
    const float LANE_WIDTH = 20.f;

    // ╔════════════════════════════════════════════════════════╗
    // ║                   VEHICLE SETTINGS                     ║
    // ╚════════════════════════════════════════════════════════╝
    const float VEHICLE_WIDTH = 15.f;
    const float VEHICLE_LENGTH = 30.f;
    
    const float NORMAL_SPEED = 120.f;
    const float EMERGENCY_SPEED = 240.f;
    const float STOPPING_DISTANCE = 50.f;

    // ╔════════════════════════════════════════════════════════╗
    // ║              TRAFFIC SIGNAL TIMINGS                    ║
    // ╚════════════════════════════════════════════════════════╝
    const float GREEN_DURATION = 10.f;
    const float YELLOW_DURATION = 2.f;
    const float CYCLE_TIME = GREEN_DURATION + YELLOW_DURATION;

    // ╔════════════════════════════════════════════════════════╗
    // ║          ADAPTIVE TIMING ALGORITHM PARAMETERS          ║
    // ╚════════════════════════════════════════════════════════╝
    const float BASE_GREEN_DURATION = 10.0f;
    const float MAX_GREEN_DURATION = 20.0f;
    const float PER_VEHICLE_EXTRA_TIME = 0.2f;
    const int VEHICLE_QUEUE_THRESHOLD = 10;

    // ╔════════════════════════════════════════════════════════╗
    // ║                     COLORS                             ║
    // ╚════════════════════════════════════════════════════════╝
    const sf::Color ROAD_COLOR(50, 50, 50);
    const sf::Color JUNCTION_COLOR(70, 70, 70);
    const sf::Color NORMAL_VEHICLE_COLOR(100, 100, 255);
    const sf::Color LANE_MARKING_COLOR(200, 200, 200);
    
    const sf::Color GREEN_LIGHT(0, 255, 0);
    const sf::Color YELLOW_LIGHT(255, 255, 0);
    const sf::Color RED_LIGHT(255, 0, 0);

    // ╔════════════════════════════════════════════════════════╗
    // ║              LIGHT POSITIONS & OFFSETS                 ║
    // ╚════════════════════════════════════════════════════════╝
    const float LIGHT_OFFSET = 100.f;

    // ╔════════════════════════════════════════════════════════╗
    // ║                EMERGENCY DETECTION                     ║
    // ╚════════════════════════════════════════════════════════╝
    const float EMERGENCY_DETECTION_RANGE = 350.f;
    const float EMERGENCY_EXIT_DISTANCE = 200.f;

    // ╔════════════════════════════════════════════════════════╗
    // ║               STATISTICS & LOGGING                     ║
    // ╚════════════════════════════════════════════════════════╝
    const float STATISTICS_UPDATE_INTERVAL = 15.f;
    const int LANE_COUNT = 4;

    // ╔════════════════════════════════════════════════════════╗
    // ║                 LANE IDENTIFIERS                       ║
    // ╚════════════════════════════════════════════════════════╝
    const int LANE_WEST = 0;
    const int LANE_NORTH = 1;
    const int LANE_EAST = 2;
    const int LANE_SOUTH = 3;
}
