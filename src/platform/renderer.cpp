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

void Renderer::draw_solid(const core::Solid& solid) {
    Vector3 pos = {
        (solid.bounds.min.x + solid.bounds.max.x) / 2,
        (solid.bounds.min.y + solid.bounds.max.y) / 2,
        (solid.bounds.min.z + solid.bounds.max.z) / 2
    };
    float w = solid.bounds.max.x - solid.bounds.min.x;
    float h = solid.bounds.max.y - solid.bounds.min.y;
    float d = solid.bounds.max.z - solid.bounds.min.z;

    // Pick color based on orientation (Doom-style flat colors)
    Color color;
    if (h < 0.3f && w > h && d > h) {
        // Flat horizontal
        if (solid.bounds.min.y < 0.5f) {
            color = {140, 140, 150, 255};  // Floor: darker gray
        } else {
            color = {220, 220, 230, 255};  // Ceiling: light gray
        }
    } else {
        color = {200, 200, 210, 255};  // Wall: off-white
    }

    DrawCube(pos, w, h, d, color);
    DrawCubeWires(pos, w, h, d, {100, 100, 110, 255});  // Subtle edges
}

void Renderer::draw_object(const core::GameObject& obj) {
    core::AABB bounds = obj.bounds();
    Vector3 pos = {
        (bounds.min.x + bounds.max.x) / 2,
        (bounds.min.y + bounds.max.y) / 2,
        (bounds.min.z + bounds.max.z) / 2
    };

    // Get color (use override or default based on type)
    core::Color4 c = obj.color.value_or(core::default_object_color(obj.type));
    Color color = {c.r, c.g, c.b, c.a};

    DrawCube(pos, obj.size.x, obj.size.y, obj.size.z, color);

    // Darker edge color for wireframe
    Color edgeColor = {
        static_cast<unsigned char>(c.r > 40 ? c.r - 40 : 0),
        static_cast<unsigned char>(c.g > 40 ? c.g - 40 : 0),
        static_cast<unsigned char>(c.b > 40 ? c.b - 40 : 0),
        c.a
    };
    DrawCubeWires(pos, obj.size.x, obj.size.y, obj.size.z, edgeColor);
}

void Renderer::draw(const core::GameState& state) {
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

    for (const auto& solid : state.world.solids) {
        draw_solid(solid);
    }

    for (const auto& obj : state.world.objects) {
        draw_object(obj);
    }

    EndMode3D();

    DrawFPS(10, 10);
    DrawText(TextFormat("Pos: %.1f, %.1f, %.1f",
        state.player.position.x, state.player.position.y, state.player.position.z),
        10, 30, 16, WHITE);

    EndDrawing();
}

} // namespace platform
