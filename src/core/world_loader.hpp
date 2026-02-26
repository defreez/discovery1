#pragma once
#include "world.hpp"
#include <string>

namespace core {

struct MapData {
    World world;
    Vec3 player_start;
    std::string name;
};

// Load world from JSON file
MapData load_map(const std::string& path);

// Load world from JSON string (for tests)
MapData load_map_from_string(const std::string& json);

} // namespace core
