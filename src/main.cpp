#include "main.hpp"

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
        std::exit(EXIT_FAILURE);
    }
    SDL_Init(SDL_INIT_AUDIO);

    int videoScale = std::stoi(argv[1]);
    int cycleDelay = std::stoi(argv[2]);
    char const* romFilename = argv[3];

    Platform platform("CHIP-8 Emulator", Display::screenWidth * videoScale, Display::screenHeight * videoScale, Display::screenWidth, Display::screenHeight);
    Display display;
    Sound sound;
    Chip chip8(display, sound);
    chip8.loadROM(romFilename);
    int videoPitch = sizeof(display.screen[0]) * Display::screenWidth;
    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    bool quit = false;
    while (!quit)
    {
        quit = platform.processInput(chip8.getKeys());
        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (dt > static_cast<float>(cycleDelay))
        {
            lastCycleTime = currentTime;
            chip8.step();
            void* buffer = display.screen.data();
            platform.update(buffer, videoPitch);
        }
    }
    return 0;
}