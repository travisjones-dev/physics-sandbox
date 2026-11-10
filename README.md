# physics-sandbox

A 2D rigid-body physics engine written from scratch in C++. [raylib](https://www.raylib.com/) is used only to open a window and draw shapes. Everything in `src/physics/` is the engine.

## Build and run

You need Visual Studio 2026 with the C++ workload installed; it includes CMake. raylib is downloaded automatically on the first build.

```powershell
.\build.ps1          # configure, build, run tests
.\build.ps1 -Run     # ...then open the sandbox
```

Controls: **Space** pause, **.** single-step while paused, **R** reset the scene, **F12** save `screenshot.png`.

## Layout

| Path | What it is |
|---|---|
| `src/physics/` | The engine: `World`, `Body`, math. It has no raylib dependency, so it can be tested without opening a window. |
| `src/main.cpp` | Window, fixed-timestep loop (60 Hz), and drawing |
| `tests/physics_tests.cpp` | Tests that check physical laws. Run them with `.\build.ps1` or `ctest`. |

World units are meters, seconds, and kilograms, with +y up. The ground line is y = 0.

## Roadmap

- [ ] **1. Integration.** Gravity, velocity, and a fixed timestep. Compare explicit and semi-implicit Euler, and show energy drift with a spring.
- [ ] **2. Circle collisions.** Circle–circle and circle–ground detection, impulse response with restitution, and positional correction.
- [ ] **3. Broadphase.** A spatial hash grid, then measure 1,000+ bodies against brute-force O(n²).
- [ ] **4. Rotation.** Orientation, angular velocity, moment of inertia, torque, and box collisions using the Separating Axis Theorem.
- [ ] **5. Stacking.** Friction, a sequential-impulse solver, and putting resting bodies to sleep.

## References

- Glenn Fiedler, "Fix Your Timestep!" and "Integration Basics" (Gaffer on Games)
- Erin Catto, GDC physics talks (box2d.org/publications)
- Christer Ericson, *Real-Time Collision Detection*
- Ian Millington, *Game Physics Engine Development*

## Benchmarks

| Balls | Brute force (ms) | Grid solution (ms) |
|---|---|---|
| 1,000 | 0.5 | |
| 3,000 | 4.37 | |
| 5,000 | 12.09 | |
