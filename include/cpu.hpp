#pragma once

#include <iostream>
#include <cstddef>
#include <array>
#include <random>
#include <vector>
#include <fstream>
#include "display.hpp"
#include "sound.hpp"
class Chip {
public:
    static constexpr uint16_t fontsStart = 0x50;
    static constexpr uint8_t fontsSize = 80;
    static constexpr uint16_t memorySize = 4096;
    static constexpr uint16_t registerCount = 16;
    static constexpr uint16_t stackSize = 16;
    static constexpr uint16_t programStart = 0x200;
    std::array<uint8_t, fontsSize> fonts =
{
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    explicit Chip(Display &display, Sound &sound);
    void step();
    void loadROM(const std::string &filepath);
    std::array<uint8_t, 16> &getKeys();
private:
    Display &display;
    Sound &sound;
    std::mt19937 rng;
    std::uniform_int_distribution<uint8_t> dist;

    std::array<uint8_t, memorySize> memory {};
    std::array<uint8_t, registerCount> registers {};
    uint16_t indexRegister {};
    uint16_t programCounter {programStart};
    std::array<uint16_t, stackSize> stack {};
    uint8_t stackPointer {};
    uint8_t delayTimer {};
    uint8_t soundTimer {};
    std::array<uint8_t, 16> keys;
    uint16_t opcode {};
    void execute();
    uint16_t get_instruction() const {
        return (static_cast<uint16_t>(memory[programCounter]) << 8) | memory[programCounter + 1];
    }

    uint16_t get_nnn_mask() const {
        return (opcode & 0x0FFF);
    }

    uint8_t get_xh_mask() const {
        return ((opcode & 0xF000) >> 12);
    }

    uint8_t get_xl_mask() const {
        return ((opcode & 0x0F00) >> 8);
    }

    uint8_t get_yh_mask() const {
        return ((opcode & 0x00F0) >> 4);
    }

    uint8_t get_yl_mask() const {
        return (opcode & 0x000F);
    }

    uint8_t get_first_byte() const {
        return ((opcode & 0xFF00) >> 8);
    }

    uint8_t get_last_byte() const {
        return (opcode & 0x00FF);
    }
};