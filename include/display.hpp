#pragma once
#include <array>

class Display {
public:
    void clear();
    static constexpr int screenWidth = 64;
    static constexpr int screenHeight = 32;
    std::array<uint32_t, screenWidth * screenHeight> screen{};
    bool draw(const std::array<uint8_t, 15> &spriteData,
              const uint8_t x, const uint8_t y, const uint8_t height);
};