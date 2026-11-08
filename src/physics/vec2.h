#pragma once
#include <cmath>

// Minimal 2D vector so the physics library doesn't depend on raylib.
// Add what you need as you go: dot, cross (2D returns a scalar), length,
// normalize, perpendicular...
struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

inline Vec2 operator+(Vec2 a, Vec2 b) {
    return {a.x + b.x, a.y + b.y};
}

inline Vec2 operator*(Vec2 c, float s) {
    return {c.x * s, c.y * s};
}

inline Vec2 operator-(Vec2 a, Vec2 b) {
    return {a.x - b.x, a.y - b.y};
}

inline float Length(Vec2 b) {
    return {std::sqrt((b.x * b.x) + (b.y * b.y))};
}

inline float Dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}