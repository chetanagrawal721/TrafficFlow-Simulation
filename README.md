# Smart Traffic Management System (v2.1)

An interactive, real-time 2D Smart Traffic Management & Junction Simulation built in **C++17** using **SFML 3.0**.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![SFML](https://img.shields.io/badge/SFML-3.0-green.svg)
![Build](https://img.shields.io/badge/Build-CMake-orange.svg)
![License](https://img.shields.io/badge/License-MIT-purple.svg)

---

## 🚦 Key Features

- **4-Way 2D Intersection Simulation**: Real-time traffic flow across West (Lane 0), North (Lane 1), East (Lane 2), and South (Lane 3).
- **Adaptive Green Timing Algorithm**: Dynamic signal adjustments based on real-time vehicle queue density:
  $$\text{Green Time} = \min(10.0 + \max(0, \text{Queue} - 10) \times 0.2, 20.0)\text{ seconds}$$
- **6-Step Emergency Vehicle Preemption**: Automatic priority detection (Ambulance, FireTruck, Police, VIP), state preservation, immediate signal override, and seamless state restoration upon clearance.
- **Classic Design Patterns Architecture**:
  - **Factory Pattern**: `VehicleFactory` for centralized vehicle instantiation.
  - **Strategy Pattern**: Interchangeable traffic timing strategies (`AdaptiveStrategy`, `FixedStrategy`, `PriorityStrategy`, `EcoStrategy`, `FairStrategy`).
  - **State Pattern**: Finite state machine (`NormalState`, `EmergencyState`, `MaintenanceState`, `CongestionState`).
  - **Observer Pattern**: `TrafficObserver` and `TrafficMonitor` for event-driven metrics logging.
  - **Decorator Pattern**: Dynamic runtime vehicle behaviors (`EcoBadge`, `PriorityBadge`, `HeavyLoad`).
  - **Composite Pattern**: Hierarchical road infrastructure (`Lane` and `JunctionComposite`).
  - **Template Method Pattern**: Standardized signal update execution cycles.
  - **Singleton Pattern**: Global system orchestration with `TrafficSystemManager`.
- **Live On-Screen HUD & Telemetry**: Dynamic overlay displaying vehicle counts, active green lanes, emergency indicators, and terminal cycle metrics.

---

## 🎮 Controls

| Key | Action | Details |
|---|---|---|
| **`A`** | Spawn Normal Car | Lane 0 (West $\rightarrow$ East) |
| **`S`** | Spawn Normal Car | Lane 1 (North $\rightarrow$ South) |
| **`D`** | Spawn Normal Car | Lane 2 (East $\rightarrow$ West) |
| **`W`** | Spawn Normal Car | Lane 3 (South $\rightarrow$ North) |
| **`E`** | Spawn Ambulance | Priority 1 (White, $240\text{ px/s}$) |
| **`F`** | Spawn Fire Truck | Priority 2 (Red, $240\text{ px/s}$) |
| **`H`** | Spawn Police Cruiser | Priority 3 (Blue, $240\text{ px/s}$) |
| **`Q`** | Spawn VIP Convoy | Priority 4 (Black, $240\text{ px/s}$) |
| **`R`** | Reset Simulation | Clears active vehicles and resets signal states |
| **`ESC`** | Exit Simulation | Terminate simulation and print final statistics report |

---

## 🛠️ Build & Installation

### Prerequisites
- **C++17** compatible compiler (GCC / Clang / MSVC)
- **CMake 3.20+**
- **SFML 3.0** library installed

### Building with CMake
```bash
# Clone the repository
git clone https://github.com/chetanagrawal721/TrafficFlow-Simulation.git
cd TrafficFlow-Simulation

# Configure CMake
cmake -B build -S .

# Build executable
cmake --build build --config Release

# Run simulation
./build/TrafficFlowSimulation
```

---

## 📊 Architecture & Simulation Details

```
                           [Lane 1: North ↓]
                                  | |
                                  | |
   [Lane 0: West →]  =============+ +=============  [Lane 2: East ←]
                                Junction (Center)
                     =============+ +=============
                                  | |
                                  | |
                           [Lane 3: South ↑]
```

### Emergency Priority Hierarchy
1. **Ambulance (P1)**: Highest priority medical emergency response.
2. **FireTruck (P2)**: Urgent fire/rescue dispatch.
3. **Police (P3)**: Law enforcement emergency vehicle.
4. **VIP Convoy (P4)**: Official state escort transport.
5. **Normal Vehicles (P5)**: Standard commuter traffic adhering to regular signal phases.

---

## 📜 License
This project is licensed under the MIT License.
