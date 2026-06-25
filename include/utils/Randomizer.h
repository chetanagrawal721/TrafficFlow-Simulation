#pragma once
#include <random>

class Randomizer {
private:
    std::mt19937 gen;
public:
    Randomizer() {
        std::random_device rd;
        gen.seed(rd());
    }

    int getInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    float getFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    bool getBool() {
        std::bernoulli_distribution dist(0.5);
        return dist(gen);
    }
};
