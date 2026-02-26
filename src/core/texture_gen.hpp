#pragma once
#include <vector>
#include <cstdint>

namespace core {

struct Color {
    uint8_t r, g, b, a;
};

struct TextureData {
    int width;
    int height;
    std::vector<Color> pixels;

    Color& at(int x, int y) { return pixels[y * width + x]; }
    const Color& at(int x, int y) const { return pixels[y * width + x]; }
};

// Generate a solid color texture
inline TextureData gen_solid(int w, int h, Color color) {
    TextureData tex{w, h, std::vector<Color>(w * h, color)};
    return tex;
}

// Generate wall texture: off-white with subtle panel lines
inline TextureData gen_wall(int w = 64, int h = 64) {
    Color base{230, 230, 235, 255};
    Color line{200, 200, 205, 255};

    TextureData tex = gen_solid(w, h, base);

    // Horizontal panel line
    for (int x = 0; x < w; x++) {
        tex.at(x, h/2) = line;
    }

    // Vertical panel line
    for (int y = 0; y < h; y++) {
        tex.at(w/2, y) = line;
    }

    return tex;
}

// Generate floor texture: gray with grid
inline TextureData gen_floor(int w = 64, int h = 64) {
    Color base{180, 180, 185, 255};
    Color line{140, 140, 145, 255};

    TextureData tex = gen_solid(w, h, base);

    // Grid lines every 16 pixels
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            if (x % 16 == 0 || y % 16 == 0) {
                tex.at(x, y) = line;
            }
        }
    }

    return tex;
}

// Generate ceiling texture: light gray, minimal detail
inline TextureData gen_ceiling(int w = 64, int h = 64) {
    Color base{240, 240, 245, 255};
    return gen_solid(w, h, base);
}

} // namespace core
