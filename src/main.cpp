// Window, fixed-timestep loop, and drawing. No physics in this file.
#include <raylib.h>
#include <rlgl.h>

#include "physics/world.h"

namespace {

constexpr int kScreenWidth = 1280;
constexpr int kScreenHeight = 720;
constexpr float kPixelsPerMeter = 50.0f;
constexpr float kGroundPixels = 60.0f;    // world y = 0 sits this far above the bottom edge
constexpr float kFixedDt = 1.0f / 60.0f;  // physics always steps 60 times per simulated second
constexpr float kMaxFrameTime = 0.25f;    // after a hitch, drop time instead of spiraling

// World space (meters, +y up, origin on the ground line at center) to screen pixels.
Vector2 ToScreen(Vec2 p) {
    return {kScreenWidth * 0.5f + p.x * kPixelsPerMeter,
            kScreenHeight - kGroundPixels - p.y * kPixelsPerMeter};
}

// raylib 5.5's TakeScreenshot applies the DPI scale twice on scaled displays,
// so read the framebuffer at its real size instead.
void SaveScreenshot(const char* path) {
    const int w = GetRenderWidth();
    const int h = GetRenderHeight();
    unsigned char* pixels = rlReadScreenPixels(w, h);
    Image image = {pixels, w, h, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    ExportImage(image, path);
    RL_FREE(pixels);
}

void ResetScene(World& world) {
    world.bodies.clear();
    for (int i = 0; i < 5; ++i) {
        Body b;
        b.position = {0.3f * i, 2.0f + 1.5f * i};
        world.bodies.push_back(b);
    }
}

}  // namespace

int main(int argc, char** argv) {
    // --screenshot: run a couple of seconds, save screenshot.png, and quit.
    const bool screenshotAndExit = argc > 1 && TextIsEqual(argv[1], "--screenshot");

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(kScreenWidth, kScreenHeight, "physics-sandbox");
    const int refresh = GetMonitorRefreshRate(GetCurrentMonitor());
    SetTargetFPS(refresh > 0 ? refresh : 60);  // the vsync hint isn't honored everywhere

    World world;
    ResetScene(world);

    bool paused = false;
    float accumulator = 0.0f;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) paused = !paused;
        if (IsKeyPressed(KEY_R)) ResetScene(world);

        // Fixed timestep: render as fast as we like, but only ever advance the
        // simulation in kFixedDt chunks (Glenn Fiedler, "Fix Your Timestep!").
        float frameTime = GetFrameTime();
        if (frameTime > kMaxFrameTime) frameTime = kMaxFrameTime;
        int steps = 0;
        if (!paused) {
            accumulator += frameTime;
            while (accumulator >= kFixedDt) {
                world.step(kFixedDt);
                accumulator -= kFixedDt;
                ++steps;
            }
        } else if (IsKeyPressed(KEY_PERIOD)) {
            world.step(kFixedDt);  // single-step while paused, handy for debugging collisions
        }

        BeginDrawing();
        ClearBackground({24, 26, 32, 255});
        DrawLineEx(ToScreen({-20.0f, 0.0f}), ToScreen({20.0f, 0.0f}), 2.0f, GRAY);
        for (const Body& b : world.bodies) {
            DrawCircleV(ToScreen(b.position), b.radius * kPixelsPerMeter, {120, 180, 255, 255});
        }
        DrawText(TextFormat("bodies: %d   steps this frame: %d   FPS: %d",
                            static_cast<int>(world.bodies.size()), steps, GetFPS()),
                 12, 12, 20, RAYWHITE);
        DrawText("SPACE pause   . step (paused)   R reset   F12 screenshot", 12, 38, 18, GRAY);
        EndDrawing();

        if (IsKeyPressed(KEY_F12)) SaveScreenshot("screenshot.png");
        if (screenshotAndExit && GetTime() > 2.0) {
            SaveScreenshot("screenshot.png");
            break;
        }
    }

    CloseWindow();
    return 0;
}
