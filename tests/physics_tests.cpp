// Tiny test runner: each test is a function, CHECK records failures, main
// returns nonzero if anything failed (so `ctest` reports it).
#include <cmath>
#include <cstdio>

#include "physics/world.h"

namespace {

int g_failures = 0;

#define CHECK(cond)                                                     \
    do {                                                                \
        if (!(cond)) {                                                  \
            std::printf("  FAILED %s:%d  %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                               \
        }                                                               \
    } while (0)

[[maybe_unused]] bool Near(float a, float b, float tolerance = 1e-4f) { return std::fabs(a - b) <= tolerance; }

void EmptyWorldSteps() {
    World world;
    for (int i = 0; i < 1000; ++i) world.step(1.0f / 60.0f);
    CHECK(world.bodies.empty());
}

void FreeFallVelocity() {
    World worldForTesting;
    Body b;
    b.position.y = 100.0f;
    worldForTesting.bodies.push_back(b);

    for (int i = 0; i < 60; i++) {
        worldForTesting.step(1.0f / 60.0f);
    }

    CHECK(Near(worldForTesting.bodies[0].velocity.y, -9.81f));
}

void BallLandsOnFloor() {
    World world;
    Body ball;
    ball.position.y = 10.0f;
    world.bodies.push_back(ball);


    // Remember, the real body lives inside the bodies vector. Ball or b or whatever
    // we call the body here inside this test is a copy.
    for (int i = 0; i < 600; i++) {
        world.step(1.0f / 60.0f);
        CHECK(world.bodies[0].position.y >= world.bodies[0].radius);
    }

    CHECK(Near(world.bodies[0].position.y, world.bodies[0].radius, 0.01f));

}

// This test is to confirm momentum is conserved.
void CollisionConservesMomentum() {
    World world;
    world.gravity = {0.0f, 0.0f};

    Body ball1;
    Body ball2;

    ball1.mass = 1.0f;
    ball1.position = {-2.0f, 5.0f};
    ball1.velocity = {3.0f, 0.0f};

    ball2.mass = 3.0f;
    ball2.position = {2.0f, 5.0f};
    ball2.velocity = {0.0f, 0.0f};

    world.bodies.push_back(ball1);
    world.bodies.push_back(ball2);

    float momentumBefore = world.bodies[0].mass * world.bodies[0].velocity.x + 
    world.bodies[1].mass * world.bodies[1].velocity.x;

    for (int i = 0; i < 200; i++) {
        world.step(1.0f / 60.0f);

        float momentumAfter = world.bodies[0].mass * world.bodies[0].velocity.x + 
        world.bodies[1].mass * world.bodies[1].velocity.x;

        CHECK(Near(momentumAfter, momentumBefore));

    }

    CHECK(world.bodies[1].velocity.x > 0);

}

// Add tests as you build each stage. Good ones assert physics laws, e.g.
//   - a body in free fall for t seconds has velocity g*t
//   - total momentum is the same before and after two balls collide
//   - total energy doesn't drift over 10,000 steps with restitution 1

}  // namespace

int main() {
    struct { const char* name; void (*fn)(); } tests[] = {
        {"EmptyWorldSteps", EmptyWorldSteps},
        {"FreeFallVelocity", FreeFallVelocity},
        {"BallLandsOnFloor", BallLandsOnFloor},
        {"CollisionConservesMomentum", CollisionConservesMomentum},
    };
    for (const auto& t : tests) {
        const int before = g_failures;
        t.fn();
        std::printf("%s %s\n", g_failures == before ? "PASS" : "FAIL", t.name);
    }
    std::printf("%d failure(s)\n", g_failures);
    return g_failures == 0 ? 0 : 1;
}
