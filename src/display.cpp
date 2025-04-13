#include "display.hpp"

void Display::clear() {
    screen.fill(0);
}

bool Display::draw(const std::array<uint8_t, 15> &spriteData,
                   const uint8_t x, const uint8_t y, const uint8_t height) {
    uint8_t xPos = x % screenWidth;
    uint8_t yPos = y % screenHeight;
    bool override = false;
    for (uint8_t i = 0; i < height; i++) {
        uint8_t value = spriteData[i];
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t spritePixel = value & (0x80u >> j);
            if (spritePixel) {
                if (screen[(xPos+j) + (yPos+i) * screenWidth] == 0xFFFFFFFF) {
                    override = true;
                }
                screen[(xPos+j) + (yPos+i) * screenWidth] ^= 0xFFFFFFFF;
            }
       }
    }
    return override;
}