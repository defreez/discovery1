#include <gtest/gtest.h>
#include "core/player.hpp"
#include <cmath>

using namespace core;

TEST(PlayerTest, InitialState) {
    Player p;
    EXPECT_FLOAT_EQ(p.position.x, 0);
    EXPECT_FLOAT_EQ(p.position.y, 1.7f);
    EXPECT_FLOAT_EQ(p.position.z, 0);
    EXPECT_FLOAT_EQ(p.yaw, 0);
    EXPECT_FLOAT_EQ(p.pitch, 0);
}

TEST(PlayerTest, ForwardDirection) {
    Player p;
    p.yaw = 0;
    p.pitch = 0;
    Vec3 fwd = p.forward();
    // Looking down -Z when yaw=0
    EXPECT_NEAR(fwd.x, 0, 0.001f);
    EXPECT_NEAR(fwd.y, 0, 0.001f);
    EXPECT_NEAR(fwd.z, -1, 0.001f);
}

TEST(PlayerTest, RightDirection) {
    Player p;
    p.yaw = 0;
    Vec3 r = p.right();
    // Right is +X when yaw=0
    EXPECT_NEAR(r.x, 1, 0.001f);
    EXPECT_NEAR(r.y, 0, 0.001f);
    EXPECT_NEAR(r.z, 0, 0.001f);
}

TEST(PlayerTest, MoveForward) {
    Player p;
    p.position = {0, 1.7f, 0};
    p.yaw = 0;

    PlayerInput input{};
    input.move_z = 1;  // Forward

    Player p2 = update_player(p, input, 1.0f);

    // Should have moved in -Z direction
    EXPECT_FLOAT_EQ(p2.position.x, 0);
    EXPECT_LT(p2.position.z, 0);  // Moved forward (-Z)
}

TEST(PlayerTest, MoveBackward) {
    Player p;
    p.position = {0, 1.7f, 0};
    p.yaw = 0;

    PlayerInput input{};
    input.move_z = -1;  // Backward

    Player p2 = update_player(p, input, 1.0f);

    EXPECT_GT(p2.position.z, 0);  // Moved backward (+Z)
}

TEST(PlayerTest, StrafeRight) {
    Player p;
    p.position = {0, 1.7f, 0};
    p.yaw = 0;

    PlayerInput input{};
    input.move_x = 1;  // Right

    Player p2 = update_player(p, input, 1.0f);

    EXPECT_GT(p2.position.x, 0);  // Moved right (+X)
}

TEST(PlayerTest, StrafeLeft) {
    Player p;
    p.position = {0, 1.7f, 0};
    p.yaw = 0;

    PlayerInput input{};
    input.move_x = -1;  // Left

    Player p2 = update_player(p, input, 1.0f);

    EXPECT_LT(p2.position.x, 0);  // Moved left (-X)
}

TEST(PlayerTest, LookYaw) {
    Player p;
    p.yaw = 0;

    PlayerInput input{};
    input.look_yaw = 0.5f;

    Player p2 = update_player(p, input, 1.0f);

    EXPECT_FLOAT_EQ(p2.yaw, 0.5f);
}

TEST(PlayerTest, LookPitchClamped) {
    Player p;
    p.pitch = 0;

    PlayerInput input{};
    input.look_pitch = 10.0f;  // Way more than max

    Player p2 = update_player(p, input, 1.0f);

    EXPECT_LE(p2.pitch, Player::MAX_PITCH);
}

TEST(PlayerTest, MovementSpeed) {
    Player p;
    p.position = {0, 1.7f, 0};
    p.yaw = 0;

    PlayerInput input{};
    input.move_z = 1;

    float dt = 1.0f;
    Player p2 = update_player(p, input, dt);

    // Distance should equal MOVE_SPEED * dt
    float dist = (p2.position - p.position).length();
    EXPECT_NEAR(dist, Player::MOVE_SPEED * dt, 0.001f);
}

TEST(PlayerTest, DiagonalMovementNormalized) {
    Player p;
    p.position = {0, 1.7f, 0};
    p.yaw = 0;

    PlayerInput input{};
    input.move_x = 1;
    input.move_z = 1;

    float dt = 1.0f;
    Player p2 = update_player(p, input, dt);

    // Diagonal movement should be same speed as straight
    float dist = (p2.position - p.position).length();
    EXPECT_NEAR(dist, Player::MOVE_SPEED * dt, 0.001f);
}

TEST(PlayerTest, Bounds) {
    Player p;
    p.position = {0, 1.7f, 0};

    AABB b = p.bounds();
    // Should be centered around player position
    EXPECT_NEAR((b.min.x + b.max.x) / 2, 0, 0.001f);
    EXPECT_NEAR((b.min.z + b.max.z) / 2, 0, 0.001f);
}
