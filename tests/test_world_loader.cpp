#include <gtest/gtest.h>
#include "core/world_loader.hpp"
#include "core/game.hpp"

using namespace core;

TEST(WorldLoaderTest, LoadFromString) {
    const char* json = R"({
        "name": "Test Room",
        "player_start": [0, 1.7, -5],
        "bounds": {
            "min": [-2, 0, -10],
            "max": [2, 3, 0]
        },
        "solids": [
            {"min": [-2, -0.1, -10], "max": [2, 0, 0]}
        ]
    })";

    MapData map = load_map_from_string(json);

    EXPECT_EQ(map.name, "Test Room");
    EXPECT_FLOAT_EQ(map.player_start.x, 0);
    EXPECT_FLOAT_EQ(map.player_start.y, 1.7f);
    EXPECT_FLOAT_EQ(map.player_start.z, -5);
    EXPECT_EQ(map.world.solids.size(), 1u);
}

TEST(WorldLoaderTest, LoadMultipleSolids) {
    const char* json = R"({
        "name": "Multi",
        "player_start": [0, 0, 0],
        "bounds": {"min": [-1, -1, -1], "max": [1, 1, 1]},
        "solids": [
            {"min": [0, 0, 0], "max": [1, 1, 1]},
            {"min": [-1, 0, 0], "max": [0, 1, 1]},
            {"min": [0, 0, -1], "max": [1, 1, 0]}
        ]
    })";

    MapData map = load_map_from_string(json);
    EXPECT_EQ(map.world.solids.size(), 3u);
}

TEST(WorldLoaderTest, CollisionAfterLoad) {
    const char* json = R"({
        "name": "Collision Test",
        "player_start": [0, 1.7, 0],
        "bounds": {"min": [-5, 0, -5], "max": [5, 3, 5]},
        "solids": [
            {"min": [-5, -0.1, -5], "max": [5, 0, 5]},
            {"min": [2, 0, -1], "max": [3, 2, 1]}
        ]
    })";

    MapData map = load_map_from_string(json);

    // Floor collision
    EXPECT_TRUE(map.world.is_solid({0, -0.05f, 0}));

    // Pillar collision
    EXPECT_TRUE(map.world.is_solid({2.5f, 1.0f, 0}));

    // Air
    EXPECT_FALSE(map.world.is_solid({0, 1.0f, 0}));
}

TEST(GameTest, MakeGameFromMap) {
    const char* json = R"({
        "name": "Game Test",
        "player_start": [1, 2, 3],
        "bounds": {"min": [-10, 0, -10], "max": [10, 5, 10]},
        "solids": []
    })";

    MapData map = load_map_from_string(json);
    GameState game = make_game(map);

    EXPECT_FLOAT_EQ(game.player.position.x, 1);
    EXPECT_FLOAT_EQ(game.player.position.y, 2);
    EXPECT_FLOAT_EQ(game.player.position.z, 3);
}

TEST(GameTest, UpdatePreventsWallClipping) {
    const char* json = R"({
        "name": "Wall Test",
        "player_start": [0, 1.7, -1],
        "bounds": {"min": [-5, 0, -10], "max": [5, 3, 0]},
        "solids": [
            {"min": [-5, 0, -10.2], "max": [5, 3, -10]}
        ]
    })";

    MapData map = load_map_from_string(json);
    GameState game = make_game(map);

    // Try to walk through the back wall
    PlayerInput input{};
    input.move_z = 1;  // Forward (toward -Z)

    // Simulate many frames of walking
    for (int i = 0; i < 1000; i++) {
        game = update_game(game, input, 0.016f);
    }

    // Player should be stopped by wall, not pass through
    EXPECT_GT(game.player.position.z, -10);
}

TEST(GameTest, TimeAccumulates) {
    const char* json = R"({
        "name": "Time Test",
        "player_start": [0, 1.7, 0],
        "bounds": {"min": [-1, 0, -1], "max": [1, 2, 1]},
        "solids": []
    })";

    MapData map = load_map_from_string(json);
    GameState game = make_game(map);

    PlayerInput input{};
    game = update_game(game, input, 0.1f);
    game = update_game(game, input, 0.2f);
    game = update_game(game, input, 0.3f);

    EXPECT_NEAR(game.time, 0.6f, 0.001f);
}
