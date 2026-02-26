#pragma once
#include "math.hpp"
#include <algorithm>

namespace core {

struct PlayerInput {
    float move_x = 0;      // -1 left, +1 right
    float move_z = 0;      // -1 back, +1 forward
    float look_yaw = 0;    // delta yaw in radians
    float look_pitch = 0;  // delta pitch in radians
};

struct Player {
    Vec3 position{0, 1.7f, 0};  // Eye height ~1.7m
    float yaw = 0;              // Radians, 0 = looking down -Z
    float pitch = 0;            // Radians, clamped to [-PI/2, PI/2]

    static constexpr float MOVE_SPEED = 3.0f;      // m/s
    static constexpr float MOUSE_SENSITIVITY = 0.002f;
    static constexpr float MAX_PITCH = 1.5f;       // ~86 degrees

    Vec3 forward() const {
        return {
            std::sin(yaw) * std::cos(pitch),
            std::sin(pitch),
            -std::cos(yaw) * std::cos(pitch)
        };
    }

    Vec3 right() const {
        return {std::cos(yaw), 0, std::sin(yaw)};
    }

    AABB bounds() const {
        // Player collision box: 0.5m wide, 1.8m tall, centered at position
        // Small epsilon above floor to prevent standing-on-floor collision
        constexpr float eps = 0.01f;
        return {
            {position.x - 0.25f, position.y - 1.7f + eps, position.z - 0.25f},
            {position.x + 0.25f, position.y + 0.1f, position.z + 0.25f}
        };
    }
};

// Pure function: updates player state, returns new state
inline Player update_player(Player p, PlayerInput input, float dt) {
    // Look
    p.yaw += input.look_yaw;
    p.pitch += input.look_pitch;
    p.pitch = std::clamp(p.pitch, -Player::MAX_PITCH, Player::MAX_PITCH);

    // Move (horizontal plane only)
    Vec3 move_dir{0, 0, 0};
    if (input.move_z != 0) {
        Vec3 fwd = p.forward();
        fwd.y = 0;
        fwd = fwd.normalized();
        move_dir += fwd * input.move_z;
    }
    if (input.move_x != 0) {
        move_dir += p.right() * input.move_x;
    }

    if (move_dir.length() > 0) {
        move_dir = move_dir.normalized();
        p.position += move_dir * Player::MOVE_SPEED * dt;
    }

    return p;
}

} // namespace core
