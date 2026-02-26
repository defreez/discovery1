#pragma once
#include "math.hpp"
#include <string>
#include <optional>
#include <cstdint>

namespace core {

struct Color4 {
    uint8_t r = 255, g = 255, b = 255, a = 255;
};

struct GameObject {
    std::string type;
    Vec3 position;
    Vec3 size;
    std::optional<Color4> color;

    AABB bounds() const {
        Vec3 half{size.x / 2, size.y / 2, size.z / 2};
        return {
            {position.x - half.x, position.y, position.z - half.z},
            {position.x + half.x, position.y + size.y, position.z + half.z}
        };
    }
};

// Default colors by object type
inline Color4 default_object_color(const std::string& type) {
    if (type == "desk") return {139, 90, 43, 255};
    if (type == "chair") return {101, 67, 33, 255};
    if (type == "computer") return {50, 50, 60, 255};
    if (type == "screen") return {0, 200, 200, 255};
    if (type == "panel") return {180, 180, 190, 255};
    if (type == "window") return {20, 20, 40, 200};
    if (type == "crate") return {194, 178, 128, 255};
    if (type == "pipe") return {160, 170, 180, 255};
    if (type == "light_fixture") return {240, 240, 250, 255};
    if (type == "bed") return {120, 130, 150, 255};
    return {200, 200, 200, 255};
}

} // namespace core
