#include "physics/world.h"

void World::step(float dt) {
    
    // Your engine starts here. See README.md for the stages.

    for (Body& b : bodies) {    // range based loop "for each body b in bodies"
        b.velocity = b.velocity + gravity*dt;
        b.position = b.position + b.velocity*dt;
    }
}
