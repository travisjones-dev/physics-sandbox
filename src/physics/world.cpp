#include "physics/world.h"
#include <cmath>

void World::step(float dt) {
    
    // Your engine starts here. See README.md for the stages.

    for (Body& b : bodies) {    // range based loop "for each body b in bodies"
        b.velocity = b.velocity + gravity*dt;
        b.position = b.position + b.velocity*dt;

        // never use == on floats
        if (b.position.y < b.radius) {
            b.position.y = b.radius;
            b.velocity.y = b.restitution * std::fabs(b.velocity.y);
        }
    }
}
