#include "core/world_loader.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

namespace core {

using json = nlohmann::json;

static Vec3 parse_vec3(const json& arr) {
    return {arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>()};
}

static AABB parse_aabb(const json& obj) {
    return {parse_vec3(obj["min"]), parse_vec3(obj["max"])};
}

static MapData parse_json(const json& j) {
    MapData data;
    data.name = j.value("name", "unnamed");
    data.player_start = parse_vec3(j["player_start"]);
    data.world.bounds = parse_aabb(j["bounds"]);

    for (const auto& solid : j["solids"]) {
        Solid s;
        s.bounds = {parse_vec3(solid["min"]), parse_vec3(solid["max"])};
        data.world.solids.push_back(s);
    }

    return data;
}

MapData load_map(const std::string& path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("Failed to open map: " + path);
    return parse_json(json::parse(f));
}

MapData load_map_from_string(const std::string& json_str) {
    return parse_json(json::parse(json_str));
}

} // namespace core
