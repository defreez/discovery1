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

static GameObject parse_object(const json& obj) {
    GameObject go;
    go.type = obj["type"].get<std::string>();
    go.position = parse_vec3(obj["position"]);
    go.size = parse_vec3(obj["size"]);

    if (obj.contains("color")) {
        const auto& c = obj["color"];
        Color4 col;
        col.r = c[0].get<uint8_t>();
        col.g = c[1].get<uint8_t>();
        col.b = c[2].get<uint8_t>();
        col.a = c.size() > 3 ? c[3].get<uint8_t>() : 255;
        go.color = col;
    }

    return go;
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

    if (j.contains("objects")) {
        for (const auto& obj : j["objects"]) {
            data.world.objects.push_back(parse_object(obj));
        }
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
