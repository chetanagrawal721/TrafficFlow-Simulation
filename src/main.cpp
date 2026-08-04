#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <optional>
#include <iomanip>

#include "core/Junction.h"
#include "core/TrafficLight.h"
#include "core/TrafficController.h"
#include "core/VehicleManager.h"
#include "entities/Vehicle.h"
#include "utils/Constants.h"
#include "utils/Vector2D.h"
#include "core/UIManager.h"
#include "core/Renderer.h"

int main() {
    try {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║     SMART TRAFFIC MANAGEMENT SYSTEM v2.1                  ║\n";
        std::cout << "║     Adaptive Timing with Vehicle Tracking                 ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
        
        std::cout << "[INIT] Starting initialization...\n\n";
        
        
        sf::RenderWindow window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), 
                                "Traffic Management v2.1 - Adaptive Timing");
        window.setFramerateLimit(Constants::TARGET_FPS);
        std::cout << "[OK] Window created (1200x900 @ 60 FPS)\n";

        
        Junction mainJunction(sf::Vector2f(Constants::JUNCTION_CENTER_X, Constants::JUNCTION_CENTER_Y));

        auto* light0 = new TrafficLight(Constants::JUNCTION_CENTER_X - Constants::LIGHT_OFFSET, 
                                         Constants::JUNCTION_CENTER_Y, 0);
        auto* light1 = new TrafficLight(Constants::JUNCTION_CENTER_X, 
                                         Constants::JUNCTION_CENTER_Y - Constants::LIGHT_OFFSET, 1);
        auto* light2 = new TrafficLight(Constants::JUNCTION_CENTER_X + Constants::LIGHT_OFFSET, 
                                         Constants::JUNCTION_CENTER_Y, 2);
        auto* light3 = new TrafficLight(Constants::JUNCTION_CENTER_X, 
                                         Constants::JUNCTION_CENTER_Y + Constants::LIGHT_OFFSET, 3);

        mainJunction.addLight(light0);
        mainJunction.addLight(light1);
        mainJunction.addLight(light2);
        mainJunction.addLight(light3);

        light0->setState(LightState::Green);
        
        std::cout << "[OK] 4 Traffic lights initialized\n";
        std::cout << "[OK] Lane 0 (West) is GREEN\n";

        
        VehicleManager vehicleManager;
        std::cout << "[OK] Vehicle manager ready\n";

        UIManager uiManager;
        uiManager.loadFont();
        std::cout << "[OK] UI manager initialized\n";

        Renderer renderer(window);
        std::cout << "[OK] Renderer initialized\n";

        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║              KEYBOARD CONTROLS                             ║\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ NORMAL VEHICLES:                                           ║\n";
        std::cout << "║   A = Lane 0 (West)    S = Lane 1 (North)                 ║\n";
        std::cout << "║   D = Lane 2 (East)    W = Lane 3 (South)                 ║\n";
        std::cout << "║                                                            ║\n";
        std::cout << "║ EMERGENCY VEHICLES:                                        ║\n";
        std::cout << "║   E = Ambulance (P1)   F = FireTruck (P2)                 ║\n";
        std::cout << "║   H = Police (P3)      Q = VIP (P4)                       ║\n";
        std::cout << "║                                                            ║\n";
        std::cout << "║ UTILITIES:                                                 ║\n";
        std::cout << "║   R = Reset            ESC = Exit                         ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";

        std::cout << "╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║           ADAPTIVE GREEN TIMING ALGORITHM                  ║\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ Base Green Time:          10 seconds                       ║\n";
        std::cout << "║ Max Green Time:           20 seconds                       ║\n";
        std::cout << "║ Vehicle Threshold:        10 vehicles                      ║\n";
        std::cout << "║ Extra Time Per Vehicle:   0.2 seconds                      ║\n";
        std::cout << "║                                                            ║\n";
        std::cout << "║ ALGORITHM EXAMPLES:                                        ║\n";
        std::cout << "║   Queue 0-10 vehicles:     10.0s green                     ║\n";
        std::cout << "║   Queue 15 vehicles:       11.0s green (+1.0s)            ║\n";
        std::cout << "║   Queue 25 vehicles:       13.0s green (+3.0s)            ║\n";
        std::cout << "║   Queue 50 vehicles:       18.0s green (+8.0s)            ║\n";
        std::cout << "║   Queue 100+ vehicles:     20.0s green (MAX - capped)     ║\n";
        std::cout << "║                                                            ║\n";
        std::cout << "║ FORMULA: green = min(10 + (queue-10) * 0.2, 20)           ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";

        std::cout << "════════════════════════════════════════════════════════════\n";
        std::cout << "                  SIMULATION STARTED\n";
        std::cout << "════════════════════════════════════════════════════════════\n\n";

        
        sf::Clock clock;
        float emergencyCheckTimer = 0.f;
        float statsPrintTimer = 0.f;
        bool running = true;
        int totalVehicles = 0;
        int emergenciesHandled = 0;
        
        bool emergencyActive = false;
        int emergencyLane = -1;
        int savedLane = 0;
        float savedTimer = 0.f;

        const auto& lights = mainJunction.getLights();

        while (window.isOpen() && running) {
            float deltaTime = clock.restart().asSeconds();

           
            
            while (std::optional<sf::Event> event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    running = false;
                    window.close();
                }

                if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPress->code == sf::Keyboard::Key::Escape) {
                        running = false;
                        window.close();
                    }
                    // Lane 0 (West)
                    else if (keyPress->code == sf::Keyboard::Key::A) {
                        vehicleManager.addNormalVehicle(0);
                        totalVehicles++;
                    }
                    // Lane 1 (North)
                    else if (keyPress->code == sf::Keyboard::Key::S) {
                        vehicleManager.addNormalVehicle(1);
                        totalVehicles++;
                    }
                    // Lane 2 (East)
                    else if (keyPress->code == sf::Keyboard::Key::D) {
                        vehicleManager.addNormalVehicle(2);
                        totalVehicles++;
                    }
                    // Lane 3 (South)
                    else if (keyPress->code == sf::Keyboard::Key::W) {
                        vehicleManager.addNormalVehicle(3);
                        totalVehicles++;
                    }
                    // Emergency vehicles
                    else if (keyPress->code == sf::Keyboard::Key::E) {
                        vehicleManager.addEmergencyVehicle(VehicleType::Ambulance);
                        totalVehicles++;
                    }
                    else if (keyPress->code == sf::Keyboard::Key::F) {
                        vehicleManager.addEmergencyVehicle(VehicleType::FireTruck);
                        totalVehicles++;
                    }
                    else if (keyPress->code == sf::Keyboard::Key::H) {
                        vehicleManager.addEmergencyVehicle(VehicleType::Police);
                        totalVehicles++;
                    }
                    else if (keyPress->code == sf::Keyboard::Key::Q) {
                        vehicleManager.addEmergencyVehicle(VehicleType::VIP);
                        totalVehicles++;
                    }
                    // Reset simulation
                    else if (keyPress->code == sf::Keyboard::Key::R) {
                        vehicleManager.clearAllVehicles();
                        emergencyActive = false;
                        emergencyLane = -1;
                        light0->setState(LightState::Green);
                        light1->setState(LightState::Red);
                        light2->setState(LightState::Red);
                        light3->setState(LightState::Red);
                        std::cout << "\n[RESET] All vehicles cleared, system reset\n\n";
                    }
                }
            }

           
            
            int laneCounts[4] = {0, 0, 0, 0};
            for (auto* v : vehicleManager.getAllVehicles()) {
                if (!v) continue;
                if (v->getIsEmergency()) continue;
                int lane = v->getLaneID();
                if (lane >= 0 && lane < 4) {
                    laneCounts[lane]++;
                }
            }
            mainJunction.getController().updateLaneCounts(laneCounts);

          
            
            if (!emergencyActive) {
                mainJunction.update(deltaTime);
            }
            
            vehicleManager.update(deltaTime);

           
            for (auto* vehicle : vehicleManager.getAllVehicles()) {
                int lane = vehicle->getLaneID();
                
                if (lane >= 0 && lane < static_cast<int>(lights.size())) {
                    vehicle->checkTrafficLight(*lights[lane]);
                }
            }
            
           
        if (!emergencyActive && vehicleManager.hasUnprocessedEmergency()) {
            EmergencyRequest request = vehicleManager.getNextUnprocessedEmergency();
            
            if (request.vehicle != nullptr) {
                std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
                std::cout << "║             EMERGENCY DETECTION & OVERRIDE                 ║\n";
                std::cout << "╚════════════════════════════════════════════════════════════╝\n";
                
                // STEP 1: DETECTION
                std::cout << "[STEP 1] EMERGENCY DETECTED:\n";
                std::cout << "  Vehicle Type Priority: " << request.priority << "\n";
                std::cout << "  Target Lane: " << request.laneID << "\n";
                std::cout << "  Vehicles in Lane: " << laneCounts[request.laneID] << "\n";
                std::cout << "  Emergency Vehicle Position: (" 
                          << std::fixed << std::setprecision(1) << request.vehicle->getPosition().getX() 
                          << ", " << request.vehicle->getPosition().getY() << ")\n";
                
                
                std::cout << "\n[STEP 2] STATE PRESERVATION:\n";
                savedLane = mainJunction.getController().getCurrentGreenLane();
                savedTimer = mainJunction.getController().getTimer();
                std::cout << "  ✓ Current Lane Saved: " << savedLane << "\n";
                std::cout << "  ✓ Timer Saved: " << std::fixed << std::setprecision(2) << savedTimer << "s\n";
                
                
                std::cout << "\n[STEP 3] IMMEDIATE SIGNAL OVERRIDE:\n";
                emergencyActive = true;
                emergencyLane = request.laneID;
                
                
                for (int i = 0; i < 4; ++i) {
                    lights[i]->setState(LightState::Red);
                }
                
                
                lights[emergencyLane]->setState(LightState::Green);
                
                std::cout << "  ✓ All lanes set to: RED\n";
                std::cout << "  ✓ Emergency lane " << emergencyLane << " set to: GREEN\n";
                std::cout << "  ✓ Normal cycle: PAUSED\n";
                std::cout << "  ✓ Current Green Lane: " << mainJunction.getController().getCurrentGreenLane() 
                          << " → Override to Lane: " << emergencyLane << "\n";
                
                
                std::cout << "\n[STEP 4] PATH CLEARANCE:\n";
                std::cout << "  • Normal vehicles speed: 120 px/s\n";
                std::cout << "  • Emergency vehicle speed: 240 px/s (2x faster)\n";
                std::cout << "  • Status: Waiting for emergency to pass...\n";
            }
        }

        
        if (emergencyActive) {
            bool emergencyPassed = true;
            bool emergencyStillExists = false;
            
            
            for (auto* vehicle : vehicleManager.getAllVehicles()) {
                if (vehicle->getIsEmergency() && vehicle->getLaneID() == emergencyLane) {
                    emergencyStillExists = true;
                    
                    Vector2D vpos = vehicle->getPosition();
                    float distToJunction = vpos.distanceTo(
                        Vector2D(Constants::JUNCTION_CENTER_X, Constants::JUNCTION_CENTER_Y)
                    );
                    
                    
                    if (distToJunction < Constants::EMERGENCY_EXIT_DISTANCE) {
                        emergencyPassed = false;
                        
                        
                        static float debugTimer = 0.f;
                        debugTimer += deltaTime;
                        if (debugTimer >= 1.0f) {
                            std::cout << "[EMERGENCY MONITORING] Lane " << emergencyLane 
                                      << " - Distance to junction: " << std::fixed 
                                      << std::setprecision(1) << distToJunction << "px\n";
                            debugTimer = 0.f;
                        }
                        break;
                    }
                }
            }

           
            if (emergencyPassed && !emergencyStillExists) {
                std::cout << "\n[STEP 5] EXIT TRACKING:\n";
                std::cout << "  ✓ Emergency vehicle distance > 200px\n";
                std::cout << "  ✓ Junction exit confirmed\n";
                
                std::cout << "\n[STEP 6] STATE RESTORATION:\n";
                std::cout << "  ✓ Restoring Lane: " << savedLane << "\n";
                std::cout << "  ✓ Restoring Timer: " << std::fixed << std::setprecision(2) << savedTimer << "s\n";
                
                emergencyActive = false;
                mainJunction.getController().setCurrentLane(savedLane);
                mainJunction.getController().setTimer(savedTimer);
                
               
                for (int i = 0; i < 4; ++i) {
                    if (i == savedLane) {
                        lights[i]->setState(LightState::Green);
                    } else {
                        lights[i]->setState(LightState::Red);
                    }
                }
                
                std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
                std::cout << "║             EMERGENCY CYCLE COMPLETE                      ║\n";
                std::cout << "║          Normal Traffic Control Resumed                   ║\n";
                std::cout << "║          Current Green Lane: " << std::setw(27) << savedLane << "║\n";
                std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
                
                emergenciesHandled++;
            }
        }


           
            statsPrintTimer += deltaTime;
            if (statsPrintTimer >= Constants::STATISTICS_UPDATE_INTERVAL) {
                statsPrintTimer = 0.f;
                
                std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
                std::cout << "║          PERIODIC TRAFFIC STATUS (Every 15 sec)           ║\n";
                std::cout << "╠════════════════════════════════════════════════════════════╣\n";
                
                std::string laneNames[] = {"WEST", "NORTH", "EAST", "SOUTH"};
                int totalQueued = 0;
                int maxQueue = 0;
                int maxLane = 0;
                
                for (int i = 0; i < 4; ++i) {
                    totalQueued += laneCounts[i];
                    if (laneCounts[i] > maxQueue) {
                        maxQueue = laneCounts[i];
                        maxLane = i;
                    }
                    
                    
                    float greenTime = Constants::BASE_GREEN_DURATION;
                    if (laneCounts[i] > Constants::VEHICLE_QUEUE_THRESHOLD) {
                        int extra = laneCounts[i] - Constants::VEHICLE_QUEUE_THRESHOLD;
                        float extraTime = Constants::PER_VEHICLE_EXTRA_TIME * extra;
                        greenTime = std::min(Constants::BASE_GREEN_DURATION + extraTime, Constants::MAX_GREEN_DURATION);
                    }
                    
                    std::cout << "║ Lane " << i << " (" << laneNames[i] << "):\n";
                    std::cout << "║   Queue Size: " << std::setw(3) << laneCounts[i] << " vehicles  |  ";
                    std::cout << "Green Time: " << std::fixed << std::setprecision(1) << std::setw(5) << greenTime << "s\n";
                }
                
                std::cout << "║\n";
                std::cout << "║ SUMMARY:\n";
                std::cout << "║   Total Vehicles Active: " << vehicleManager.getVehicleCount() << "\n";
                std::cout << "║   Total Emergencies: " << vehicleManager.getEmergencyCount() << "\n";
                std::cout << "║   Busiest Lane: " << maxLane << " (" << laneNames[maxLane] << ") - " << maxQueue << " vehicles\n";
                std::cout << "║   Current Green Lane: " << mainJunction.getController().getCurrentGreenLane() << "\n";
                
                if (emergencyActive) {
                    std::cout << "║   Status: 🚨 EMERGENCY OVERRIDE ACTIVE (Lane " << emergencyLane << ")\n";
                } else {
                    std::cout << "║   Status: ✓ Normal Operation\n";
                }
                
                std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
            }

          
            renderer.drawAll(mainJunction, vehicleManager);
            uiManager.draw(window, vehicleManager.getVehicleCount(),
                          vehicleManager.getEmergencyCount(),
                          mainJunction.getController().getCurrentGreenLane(),
                          emergencyActive);
            window.display();
        }

        // ╔════════════════════════════════════════════════════════╗
        // ║                  CLEANUP                              ║
        // ╚════════════════════════════════════════════════════════╝
        
        delete light0;
        delete light1;
        delete light2;
        delete light3;

        // ╔════════════════════════════════════════════════════════╗
        // ║               FINAL STATISTICS REPORT                  ║
        // ╚════════════════════════════════════════════════════════╝
        
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                  SIMULATION TERMINATED                      ║\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ Total Vehicles Spawned: " << std::setw(27) << totalVehicles << "║\n";
        std::cout << "║ Emergencies Handled: " << std::setw(31) << emergenciesHandled << "║\n";
        std::cout << "║ Final Vehicle Count: " << std::setw(31) << vehicleManager.getVehicleCount() << "║\n";
        std::cout << "║ Status: ✓ SUCCESS\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cout << "[FATAL ERROR] " << e.what() << std::endl;
        return 1;
    }
}
