# Discovery One - Project Instructions

## Git Workflow

**Always create a branch based on the GitHub issue before working on it.**

Branch naming convention: `issue-<number>-<short-description>`

**Commit often, squash merge at the end.**
- Make many small commits as you work
- Push frequently
- When PR is ready, squash merge to keep main history clean

Example workflow:
```bash
# Before starting work on issue #3
git checkout main
git pull origin main
git checkout -b issue-3-first-person-camera

# Do work, commit frequently
git add <files>
git commit -m "Add camera struct"
git commit -m "Implement mouse look"
git commit -m "Add WASD movement"
git push -u origin issue-3-first-person-camera

# Create PR, then squash merge
gh pr create --title "First-person camera" --body "Closes #3"
gh pr merge --squash
```

## Repository

GitHub: https://github.com/defreez/discovery1

## Tech Stack

- C++20
- raylib 5.5 (fetched via CMake FetchContent)
- Procedural geometry (no external 3D models)
- AI-generated textures

## Architecture: Headless-First

**The entire game runs without rendering. Renderer is just a viewer.**

- Player movement, collision, spatial queries - all headless
- Drive through the ship, interact with geometry - no window needed
- Core has ZERO raylib dependencies
- Tests exercise the full 3D simulation

**All map data in JSON.** Walls, architecture, everything - no hardcoded geometry.

```
src/
  core/           # Pure C++, no raylib
    math.hpp        # Vec3, Mat4, Quat
    player.hpp      # Position, orientation, movement
    world.hpp       # Ship geometry, collision, spatial queries
    world_loader.hpp # Load world from JSON
    game.hpp        # Full game state, update loop
  platform/       # raylib wrapper (thin)
    renderer.cpp    # Visualizes game state
    input.cpp       # Converts raylib input to core types
  main.cpp        # Entry point only
maps/
  test_corridor.json  # Test map
tests/
  test_*.cpp      # Headless tests of core/
```

## Testing

**Headless tests only.** Do not run the GUI app - you can't see it and gain nothing.

```bash
cmake --build build && cd build && ctest --output-on-failure
```

When visual verification is needed, write a test plan for the user:
```
VISUAL TEST PLAN:
1. Run ./build/discovery1
2. Verify: Player starts inside corridor
3. Verify: WASD moves player
4. Verify: Mouse look works
5. Verify: Walls block movement
```

## Build Commands

```bash
cmake -B build
cmake --build build
./build/discovery1
```
