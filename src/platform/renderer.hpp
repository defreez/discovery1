#pragma once
#include "core/game.hpp"
#include "raylib.h"

namespace platform {

class Renderer {
public:
    void init(int width, int height, const char* title);
    void shutdown();
    void draw(const core::GameState& state);
    bool should_close() const;

private:
    Camera3D camera_{};
    void draw_solid(const core::Solid& solid);
};

} // namespace platform
