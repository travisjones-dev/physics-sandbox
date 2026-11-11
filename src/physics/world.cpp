#include "physics/world.h"
#include <cmath>
#include <algorithm>

void World::step(float dt) {
    
    // Your engine starts here. See README.md for the stages.


    // This loop creates gravity and creates the bounce + settle for the 2D engine.
    for (Body& b : bodies) {    // range based loop "for each body b in bodies"
        // Gravity implemented using semi-implicit euler (velocity first)
        // This prevents energy from drifting (which will cause crashes and "blow ups")
        b.velocity = b.velocity + gravity*dt;
        b.position = b.position + b.velocity*dt;

        // never use == on floats
        // Essentially creates floor and bounce effect upon colliding with floor
        if (b.position.y < b.radius) {
            b.position.y = b.radius;
            b.velocity.y = b.restitution * std::fabs(b.velocity.y);
        }

        // Creates boundary for positive position wall
        if (b.position.x >= rightWall - b.radius) {
            b.position.x = rightWall - b.radius;
            b.velocity.x = -b.restitution * std::fabs(b.velocity.x); // remember fabs makes the velocity always positive
                                                                    // do we want a positive velocity here?
        }

        // Creates boundary for negative position wall
        if (b.position.x <= leftWall + b.radius) {
            b.position.x = leftWall + b.radius;
            b.velocity.x = b.restitution * std::fabs(b.velocity.x);
        }
    }


    // Quick note here: You always want the starting point
    // and your condition to have the same type. .size() returns type size_t
    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            Body& a = bodies[i];
            Body& b = bodies[j];
            
            // the actual distance
            float distance = Length(a.position - b.position);


            if (distance > 0.0001f && distance < a.radius + b.radius) {
                float overlap = (a.radius + b.radius) - distance; 
                Vec2 normal = (b.position - a.position) * (1.0f / distance); 

                // push the objects apart.
                a.position = a.position - normal*(overlap / a.mass);
                b.position = b.position + normal*(overlap / b.mass);

                // Relative velocity
                Vec2 relVelocity = b.velocity - a.velocity;
                float closingSpeed = Dot(relVelocity, normal);

                if (closingSpeed < 0) {
                    float minRestitution = std::min(a.restitution, b.restitution);
                    // Impulse is an instant change in momentum. Instead of simulating collision forces over time, 
                    // I compute the kick needed to reverse the closing velocity, scaled by restitution, 
                    // and apply it equally and oppositely to both bodies, divided by each one's mass.
                    float impulse = -(1 + minRestitution) * (closingSpeed / (1 / a.mass + 1 / b.mass));

                    
                    b.velocity = b.velocity + normal * (impulse / b.mass);
                    a.velocity = a.velocity - normal * (impulse / a.mass);
                  
                }
            }

        }
    }


}
