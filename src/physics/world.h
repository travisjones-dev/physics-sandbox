#pragma once

#include <vector>

#include "physics/vec2.h"

// World units are meters, seconds, and kilograms. +y is up.
struct Body {
    Vec2 position;
    float radius = 0.5f;
    // Stage 1: add the state the integrator needs (velocity, mass or inverse mass, ...).
    Vec2 velocity{0.0f, 0.0f};

    // restitution is a property of the material, e.g. a
    // basketball has a restitution of 0.8
    // meaning the basketball bounces back after 
    // hitting the ground at 80% of its velocity upon impact.
    float restitution = 0.8f;

    // Implementing mass field for varying mass. unit is kg.
    float mass = 1.0f;

};

class World {
public:
    // Advance the simulation by exactly dt seconds. Called at a fixed rate by
    // main.cpp, so you can assume dt never changes.
    void step(float dt);

    std::vector<Body> bodies;
    Vec2 gravity{0.0f, -9.81f};

    float leftWall = -1000.0f;
    float rightWall = 1000.0f;
};
