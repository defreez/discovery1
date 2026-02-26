#pragma once
#include "player.hpp"
#include "world.hpp"
#include "world_loader.hpp"

namespace core {

struct GameState {
    Player player;
    World world;
    float time = 0;
};

// Pure function: returns new game state
inline GameState update_game(GameState state, PlayerInput input, float dt) {
    Vec3 old_pos = state.player.position;

    state.player = update_player(state.player, input, dt);

    // Collision: if new position collides, revert
    if (state.world.collides(state.player.bounds())) {
        state.player.position = old_pos;
    }

    state.time += dt;
    return state;
}

// Create game from map file
inline GameState make_game(const std::string& map_path) {
    MapData map = load_map(map_path);
    GameState state;
    state.world = map.world;
    state.player.position = map.player_start;
    state.player.position.y = map.player_start.y;  // Use map's eye height
    return state;
}

// Create game from map data (for tests)
inline GameState make_game(const MapData& map) {
    GameState state;
    state.world = map.world;
    state.player.position = map.player_start;
    return state;
}

} // namespace core
