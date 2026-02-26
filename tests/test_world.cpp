#include <gtest/gtest.h>
#include "core/world.hpp"

using namespace core;

class WorldTest : public ::testing::Test {
protected:
    World world;

    void SetUp() override {
        // Simple room: 4x3x4 meters, walls on all sides
        float w = 2, h = 1.5f, d = 2;

        // Floor
        world.solids.push_back({{Vec3{-w, -0.1f, -d}, Vec3{w, 0, d}}});
        // Ceiling
        world.solids.push_back({{Vec3{-w, h, -d}, Vec3{w, h + 0.1f, d}}});
        // Left wall
        world.solids.push_back({{Vec3{-w - 0.1f, 0, -d}, Vec3{-w, h, d}}});
        // Right wall
        world.solids.push_back({{Vec3{w, 0, -d}, Vec3{w + 0.1f, h, d}}});
        // Back wall
        world.solids.push_back({{Vec3{-w, 0, -d - 0.1f}, Vec3{w, h, -d}}});
        // Front wall
        world.solids.push_back({{Vec3{-w, 0, d}, Vec3{w, h, d + 0.1f}}});

        world.bounds = {{-w, 0, -d}, {w, h, d}};
    }
};

TEST_F(WorldTest, IsSolidInWall) {
    // Point inside left wall
    EXPECT_TRUE(world.is_solid({-2.05f, 0.5f, 0}));
}

TEST_F(WorldTest, IsSolidInAir) {
    // Point in middle of room
    EXPECT_FALSE(world.is_solid({0, 0.5f, 0}));
}

TEST_F(WorldTest, IsSolidOnFloor) {
    // Point inside floor
    EXPECT_TRUE(world.is_solid({0, -0.05f, 0}));
}

TEST_F(WorldTest, CollidesWithWall) {
    // Box touching left wall
    AABB box{{-2.1f, 0.5f, -0.1f}, {-1.9f, 0.6f, 0.1f}};
    EXPECT_TRUE(world.collides(box));
}

TEST_F(WorldTest, NoCollisionInAir) {
    // Box in middle of room
    AABB box{{-0.1f, 0.5f, -0.1f}, {0.1f, 0.6f, 0.1f}};
    EXPECT_FALSE(world.collides(box));
}

TEST_F(WorldTest, RaycastHitsWall) {
    // Ray from center toward left wall
    auto hit = world.raycast({0, 0.5f, 0}, {-1, 0, 0});
    ASSERT_TRUE(hit.has_value());
    EXPECT_NEAR(hit->distance, 2.0f, 0.1f);
    EXPECT_NEAR(hit->normal.x, 1, 0.001f);  // Normal points back at us
}

TEST_F(WorldTest, RaycastMissesWithWrongDirection) {
    // Ray pointing up inside room
    auto hit = world.raycast({0, 0.5f, 0}, {0, 1, 0});
    ASSERT_TRUE(hit.has_value());  // Will hit ceiling
    EXPECT_NEAR(hit->distance, 1.0f, 0.1f);
}

TEST_F(WorldTest, RaycastFromOutsideWorld) {
    // Ray from way outside
    auto hit = world.raycast({100, 0.5f, 0}, {1, 0, 0});
    EXPECT_FALSE(hit.has_value());
}

TEST_F(WorldTest, VisibleSurfacesInRoom) {
    // Standing in middle looking forward
    auto surfaces = world.visible_surfaces({0, 0.75f, 0}, {0, 0, -1}, 1.5f, 1.0f, 5, 5);
    EXPECT_GT(surfaces.size(), 0u);
}
