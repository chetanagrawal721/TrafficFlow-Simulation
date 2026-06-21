cmake_minimum_required(VERSION 3.20)
project(TrafficFlowSimulation VERSION 0.1 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")


set(SOURCE_FILES
    src/main.cpp
    src/core/TrafficController.cpp
    src/entities/Vehicle.cpp
)


add_executable(TrafficFlowSimulation ${SOURCE_FILES} ${HEADERS})



find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

target_link_libraries(TrafficFlowSimulation PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio
)


target_include_directories(TrafficFlowSimulation PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)


set_target_properties(TrafficFlowSimulation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)


if(MSVC)
    # Visual Studio compiler
    target_compile_options(TrafficFlowSimulation PRIVATE /W4)
else()
    # GCC/Clang compiler (MinGW)
    target_compile_options(TrafficFlowSimulation PRIVATE -Wall -Wextra -Wpedantic)
endif()


include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
    set_property(TARGET TrafficFlowSimulation PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()


message(STATUS "========================================================")
message(STATUS "  TRAFFIC FLOW SIMULATION BUILD CONFIGURATION")
message(STATUS "========================================================")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Source Directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary Directory: ${CMAKE_BINARY_DIR}")
message(STATUS "========================================================")
