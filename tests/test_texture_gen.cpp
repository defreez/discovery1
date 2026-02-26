#include <gtest/gtest.h>
#include "core/texture_gen.hpp"

using namespace core;

TEST(TextureGenTest, SolidColor) {
    auto tex = gen_solid(8, 8, {255, 0, 0, 255});
    EXPECT_EQ(tex.width, 8);
    EXPECT_EQ(tex.height, 8);
    EXPECT_EQ(tex.pixels.size(), 64u);

    // All pixels should be red
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            EXPECT_EQ(tex.at(x, y).r, 255);
            EXPECT_EQ(tex.at(x, y).g, 0);
            EXPECT_EQ(tex.at(x, y).b, 0);
        }
    }
}

TEST(TextureGenTest, WallHasLines) {
    auto tex = gen_wall(64, 64);
    EXPECT_EQ(tex.width, 64);
    EXPECT_EQ(tex.height, 64);

    // Center lines should be darker than base
    Color base = tex.at(0, 0);
    Color hline = tex.at(0, 32);
    Color vline = tex.at(32, 0);

    EXPECT_LT(hline.r, base.r);
    EXPECT_LT(vline.r, base.r);
}

TEST(TextureGenTest, FloorHasGrid) {
    auto tex = gen_floor(64, 64);

    // Grid lines at x=0, x=16, etc should be darker
    Color base = tex.at(1, 1);
    Color grid = tex.at(0, 1);

    EXPECT_LT(grid.r, base.r);
}

TEST(TextureGenTest, CeilingIsSolid) {
    auto tex = gen_ceiling(64, 64);

    // All pixels should be the same
    Color first = tex.at(0, 0);
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            EXPECT_EQ(tex.at(x, y).r, first.r);
            EXPECT_EQ(tex.at(x, y).g, first.g);
            EXPECT_EQ(tex.at(x, y).b, first.b);
        }
    }
}
