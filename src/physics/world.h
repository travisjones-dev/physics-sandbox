#pragma once

#include <vector>

#include "physics/vec2.h"

// World units are meters, seconds, and kilograms. +y is up.
struct Body {
    Vec2 position;
    float radius = 0.5f;
    // Stage 1: add the state the integrator needs (velocity, mass or inverse mass, ...).
};

class World {
public:
    // Advance the simulation by exactly dt seconds. Called at a fixed rate by
    // main.cpp, so you can assume dt never changes.
    void step(float dt);

    std::vector<Body> bodies;
    Vec2 gravity{0.0f, -9.81f};
};
