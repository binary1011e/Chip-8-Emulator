#pragma once

#include <SDL.h>
#include <array>
class Platform {
public:
    Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
    ~Platform();
    void update(void const* buffer, int pitch);
    bool processInput(std::array<uint8_t, 16> &keys);

private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};
};