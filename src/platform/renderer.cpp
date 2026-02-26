#include "platform/renderer.hpp"

namespace platform {

void Renderer::init(int width, int height, const char* title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);
    DisableCursor();

    camera_.position = {0, 1.7f, 0};
    camera_.target = {0, 1.7f, -1};
    camera_.up = {0, 1, 0};
    camera_.fovy = 75.0f;
    camera_.projection = CAMERA_PERSPECTIVE;
}

void Renderer::shutdown() {
    CloseWindow();
}

bool Renderer::should_close() const {
    return WindowShouldClose();
}

void Renderer::draw(const core::GameState& state) {
    // Update camera from player state
    camera_.position = {state.player.position.x, state.player.position.y, state.player.position.z};

    core::Vec3 fwd = state.player.forward();
    camera_.target = {
        camera_.position.x + fwd.x,
        camera_.position.y + fwd.y,
        camera_.position.z + fwd.z
    };

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera_);

    // Draw all solids as white boxes
    for (const auto& solid : state.world.solids) {
        Vector3 pos = {
            (solid.bounds.min.x + solid.bounds.max.x) / 2,
            (solid.bounds.min.y + solid.bounds.max.y) / 2,
            (solid.bounds.min.z + solid.bounds.max.z) / 2
        };
        Vector3 size = {
            solid.bounds.max.x - solid.bounds.min.x,
            solid.bounds.max.y - solid.bounds.min.y,
            solid.bounds.max.z - solid.bounds.min.z
        };
        DrawCubeWires(pos, size.x, size.y, size.z, WHITE);
    }

    // Draw floor grid for reference
    DrawGrid(20, 1.0f);

    EndMode3D();

    // Debug info
    DrawFPS(10, 10);
    DrawText(TextFormat("Pos: %.1f, %.1f, %.1f",
        state.player.position.x, state.player.position.y, state.player.position.z),
        10, 30, 16, WHITE);

    EndDrawing();
}

} // namespace platform
