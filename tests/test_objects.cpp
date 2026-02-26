#include <gtest/gtest.h>
#include "core/object.hpp"
#include "core/world.hpp"
#include "core/world_loader.hpp"
#include "core/game.hpp"

using namespace core;

TEST(GameObjectTest, BoundsCalculation) {
    GameObject obj;
    obj.type = "desk";
    obj.position = {0, 0, -5};
    obj.size = {0.8f, 0.7f, 0.4f};

    AABB bounds = obj.bounds();

    // Position is at base center, so:
    // X: -0.4 to 0.4 (half of 0.8)
    // Y: 0 to 0.7 (full height from base)
    // Z: -5.2 to -4.8 (half of 0.4)
    EXPECT_FLOAT_EQ(bounds.min.x, -0.4f);
    EXPECT_FLOAT_EQ(bounds.max.x, 0.4f);
    EXPECT_FLOAT_EQ(bounds.min.y, 0.0f);
    EXPECT_FLOAT_EQ(bounds.max.y, 0.7f);
    EXPECT_FLOAT_EQ(bounds.min.z, -5.2f);
    EXPECT_FLOAT_EQ(bounds.max.z, -4.8f);
}

TEST(GameObjectTest, DefaultColors) {
    EXPECT_EQ(default_object_color("desk").r, 139);
    EXPECT_EQ(default_object_color("chair").r, 101);
    EXPECT_EQ(default_object_color("window").a, 200);  // Semi-transparent
    EXPECT_EQ(default_object_color("unknown").r, 200);  // Default gray
}

TEST(ObjectCollisionTest, WorldCollidesWithObjects) {
    World world;
    world.bounds = {{-5, 0, -10}, {5, 3, 0}};

    // Add a desk object
    GameObject desk;
    desk.type = "desk";
    desk.position = {0, 0, -5};
    desk.size = {0.8f, 0.7f, 0.4f};
    world.objects.push_back(desk);

    // Box inside the desk bounds should collide
    AABB inside_desk{{-0.1f, 0.1f, -5.1f}, {0.1f, 0.3f, -4.9f}};
    EXPECT_TRUE(world.collides(inside_desk));

    // Box completely outside should not collide
    AABB outside{{2.0f, 0.1f, -5.1f}, {2.2f, 0.3f, -4.9f}};
    EXPECT_FALSE(world.collides(outside));
}

TEST(GameTest, PlayerCollidesWithObjects) {
    const char* json = R"({
        "name": "Object Collision",
        "player_start": [0, 1.7, -3],
        "bounds": {"min": [-5, 0, -10], "max": [5, 3, 0]},
        "solids": [
            {"min": [-5, -0.1, -10], "max": [5, 0, 0]}
        ],
        "objects": [
            {"type": "crate", "position": [0, 0, -6], "size": [1, 1, 1]}
        ]
    })";

    MapData map = load_map_from_string(json);
    GameState game = make_game(map);

    // Try to walk toward the crate
    PlayerInput input{};
    input.move_z = 1;  // Forward (toward -Z)

    // Simulate walking toward the crate
    for (int i = 0; i < 200; i++) {
        game = update_game(game, input, 0.016f);
    }

    // Player should be stopped by crate, not pass through
    // Crate is at Z=-6, size 1, so min.z = -6.5
    // Player should stop before reaching inside the crate
    EXPECT_GT(game.player.position.z, -6.5f);
}
