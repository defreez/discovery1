#include <gtest/gtest.h>
#include "core/math.hpp"

using namespace core;

TEST(Vec3Test, Addition) {
    Vec3 a{1, 2, 3};
    Vec3 b{4, 5, 6};
    Vec3 c = a + b;
    EXPECT_FLOAT_EQ(c.x, 5);
    EXPECT_FLOAT_EQ(c.y, 7);
    EXPECT_FLOAT_EQ(c.z, 9);
}

TEST(Vec3Test, Subtraction) {
    Vec3 a{4, 5, 6};
    Vec3 b{1, 2, 3};
    Vec3 c = a - b;
    EXPECT_FLOAT_EQ(c.x, 3);
    EXPECT_FLOAT_EQ(c.y, 3);
    EXPECT_FLOAT_EQ(c.z, 3);
}

TEST(Vec3Test, ScalarMultiply) {
    Vec3 a{1, 2, 3};
    Vec3 b = a * 2;
    EXPECT_FLOAT_EQ(b.x, 2);
    EXPECT_FLOAT_EQ(b.y, 4);
    EXPECT_FLOAT_EQ(b.z, 6);
}

TEST(Vec3Test, Length) {
    Vec3 a{3, 4, 0};
    EXPECT_FLOAT_EQ(a.length(), 5);
}

TEST(Vec3Test, Normalized) {
    Vec3 a{3, 0, 0};
    Vec3 n = a.normalized();
    EXPECT_FLOAT_EQ(n.x, 1);
    EXPECT_FLOAT_EQ(n.y, 0);
    EXPECT_FLOAT_EQ(n.z, 0);
}

TEST(Vec3Test, Dot) {
    Vec3 a{1, 0, 0};
    Vec3 b{0, 1, 0};
    EXPECT_FLOAT_EQ(dot(a, b), 0);  // Perpendicular

    Vec3 c{1, 0, 0};
    Vec3 d{1, 0, 0};
    EXPECT_FLOAT_EQ(dot(c, d), 1);  // Parallel
}

TEST(Vec3Test, Cross) {
    Vec3 x{1, 0, 0};
    Vec3 y{0, 1, 0};
    Vec3 z = cross(x, y);
    EXPECT_FLOAT_EQ(z.x, 0);
    EXPECT_FLOAT_EQ(z.y, 0);
    EXPECT_FLOAT_EQ(z.z, 1);
}

TEST(AABBTest, Contains) {
    AABB box{{0, 0, 0}, {1, 1, 1}};
    EXPECT_TRUE(box.contains({0.5f, 0.5f, 0.5f}));
    EXPECT_TRUE(box.contains({0, 0, 0}));
    EXPECT_TRUE(box.contains({1, 1, 1}));
    EXPECT_FALSE(box.contains({1.1f, 0.5f, 0.5f}));
    EXPECT_FALSE(box.contains({-0.1f, 0.5f, 0.5f}));
}

TEST(AABBTest, Intersects) {
    AABB a{{0, 0, 0}, {1, 1, 1}};
    AABB b{{0.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 1.5f}};
    EXPECT_TRUE(a.intersects(b));

    AABB c{{2, 2, 2}, {3, 3, 3}};
    EXPECT_FALSE(a.intersects(c));
}
