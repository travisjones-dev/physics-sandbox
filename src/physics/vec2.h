#pragma once

// Minimal 2D vector so the physics library doesn't depend on raylib.
// Add what you need as you go: dot, cross (2D returns a scalar), length,
// normalize, perpendicular...
struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};
