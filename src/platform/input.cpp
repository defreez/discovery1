#include "platform/input.hpp"
#include "raylib.h"

namespace platform {

core::PlayerInput read_input() {
    core::PlayerInput input{};

    // Movement
    if (IsKeyDown(KEY_W)) input.move_z = 1;
    if (IsKeyDown(KEY_S)) input.move_z = -1;
    if (IsKeyDown(KEY_A)) input.move_x = -1;
    if (IsKeyDown(KEY_D)) input.move_x = 1;

    // Combine if both pressed
    if (IsKeyDown(KEY_W) && IsKeyDown(KEY_S)) input.move_z = 0;
    if (IsKeyDown(KEY_A) && IsKeyDown(KEY_D)) input.move_x = 0;

    // Mouse look
    Vector2 delta = GetMouseDelta();
    input.look_yaw = delta.x * core::Player::MOUSE_SENSITIVITY;
    input.look_pitch = -delta.y * core::Player::MOUSE_SENSITIVITY;  // Inverted Y

    return input;
}

} // namespace platform
