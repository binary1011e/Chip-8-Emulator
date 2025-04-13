#include "cpu.hpp"

Chip::Chip(Display &display, Sound &sound) :
            display(display),
            sound(sound),
            rng(std::random_device{}()),
            dist(0, 255),
            keys(std::array<uint8_t, 16>{}) {
    programCounter = programStart;
    for (unsigned int i = 0; i < fontsSize; ++i)
    {
        memory[fontsStart + i] = fonts[i];
    }
}

std::array<uint8_t, 16> &Chip::getKeys() {
    return keys;
}


void Chip::loadROM(const std::string &filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate); // open at end
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    std::streamsize size = file.tellg();       // get file size
    file.seekg(0, std::ios::beg);              // rewind to start

    std::vector<uint8_t> buffer(size);         // allocate
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + filepath);
    }
    for (long i = 0; i < size; i++){
        memory[programStart + i] = buffer[i];
    }
}
void Chip::execute() {
    opcode = get_instruction();
    programCounter += 2;
    const uint16_t nnnMask = get_nnn_mask();
    const uint8_t xh = get_xh_mask();
    const uint8_t xl = get_xl_mask();
    const uint8_t yh = get_yh_mask();
    const uint8_t yl = get_yl_mask();
    const uint8_t lowerByte = get_last_byte();

    switch (xh) {
        case 0x0: {
            if (lowerByte == 0xEE) {
                stackPointer--;
                programCounter = stack[stackPointer];
            } else {
                display.clear();
            }
            break;
        }
        case 0x1: {
            programCounter = nnnMask;
            break;
        }
        case 0x2: {
            stack[stackPointer] = programCounter;
            stackPointer++;
            programCounter = nnnMask;
            break;
        }
        case 0x3: {
            if (registers[xl] == lowerByte) {
                programCounter += 2;
            }
            break;
        }
        case 0x4: {
            if (registers[xl] != lowerByte) {
                programCounter += 2;
            }
            break;
        }
        case 0x5: {
            if (registers[xl] == registers[yh]) {
                programCounter += 2;
            }
            break;
        }
        case 0x6: {
            registers[xl] = lowerByte;
            break;
        }
        case 0x7: {
            registers[xl] += lowerByte;
            break;
        }
        case 0x8: {
            switch (yl) {
                case 0x0: {
                    registers[xl] = registers[yh];
                    break;
                }
                case 0x1: {
                    registers[xl] |= registers[yh];
                    break;
                }
                case 0x2: {
                    registers[xl] &= registers[yh];
                    break;
                }
                case 0x3: {
                    registers[xl] ^= registers[yh];
                    break;
                }
                case 0x4: {
                    uint16_t sum = registers[xl] + registers[yh];
                    registers[0xF] = (sum > 0xFF) ? 1 : 0;
                    registers[xl] = sum & 0xFF;
                    break;
                }
                case 0x5: {
                    if (registers[xl] > registers[yh]) {
                        registers[0xF] = 1;
                    } else {
                        registers[0xF] = 0;
                    }
                    registers[xl] -= registers[yh];
                    break;
                }
                case 0x6: {
                    registers[0xF] = registers[xl] & 0x1;
                    registers[xl] = (registers[xl] >> 1);
                    break;
                }
                case 0x7: {
                    if (registers[yh] > registers[xl]) {
                        registers[0xF] = 1;
                    } else {
                        registers[0xF] = 0;
                    }
                    registers[xl] = registers[yh] - registers[xl];
                    break;
                }
                default: {
                    if (registers[xl] & 0x80) {
                        registers[0xF] = 1;
                    } else {
                        registers[0xF] = 0;
                    }
                    registers[xl] = (registers[xl] << 1);
                    break;
                }
            }
            break;
        }
        case 0x9: {
            if (registers[xl] != registers[yh]) {
                programCounter += 2;
            }
            break;
        }
        case 0xA: {
            indexRegister = nnnMask;
            break;
        }
        case 0xB: {
            programCounter = nnnMask + registers[0];
            break;
        }
        case 0xC: {
            registers[xl] = (dist(rng) & lowerByte);
            break;
        }
        case 0xD: {
            std::array<uint8_t, 15> spriteData{};
            if (static_cast<int>(registers[xl]) > 48) {
            }
            for (int i = 0; i < yl; i++) {
                spriteData[i] = memory[indexRegister + i];
            }
            registers[0xF] = display.draw(spriteData, registers[xl], registers[yh], yl);
            break;
        }
        case 0xE: {
            if (lowerByte == 0x9E) {
                programCounter += 2 * keys.at(registers[xl]);
            } else {
                programCounter += 2 * (1 - keys[registers[xl]]);
            }
            break;
        }
        default: {
            switch (lowerByte) {
                case 0x07: {
                    registers[xl] = delayTimer;
                    break;
                }
                case 0x0A: {
                    bool pressed = false;
                    for (int i = 0; i < 15; i++) {
                        if (keys[i]) {
                            registers[xl] = i;
                            pressed = true;
                            break;
                        }
                    }
                    if (!pressed) {
                        programCounter -= 2;
                    }
                    break;
                }
                case 0x15: {
                    delayTimer = registers[xl];
                    break;
                }
                case 0x18: {
                    soundTimer = registers[xl];
                    break;
                }
                case 0x1E: {
                    indexRegister += registers[xl];
                    break;
                }
                case 0x29: {
                    indexRegister = fontsStart + 5 * registers[xl];
                    break;
                }
                case 0x33: {
                    uint8_t value = registers[xl];
                    memory[indexRegister + 2] = value % 10;
                    value /= 10;
                    memory[indexRegister + 1] = value % 10;
                    value /= 10;
                    memory[indexRegister] = value % 10;
                    break;
                }
                case 0x55: {
                    for (int i = 0; i <= xl; i++) {
                        memory[indexRegister + i] = registers[i];
                    }
                    break;
                }
                default: {
                    for (int i = 0; i <= xl; i++) {
                        registers[i] = memory[indexRegister + i];
                    }
                }
            }
        }
    }
}

void Chip::step() {
    execute();

    if (delayTimer > 0)
    {
        delayTimer--;
    }

    if (soundTimer > 0)
    {
        sound.play();
        soundTimer--;
    } else {
        sound.stop();
    }
}