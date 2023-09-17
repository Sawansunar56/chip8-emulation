#include "CHIP8/cpu.h"
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;

const unsigned int FONT_START = 0x50;

unsigned char chip8_fontset[80] = {
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

chip8::chip8()
    : randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
    pc = 0x200;
    opcode = 0x000;
    I = 0;
    sp = 0;

    for (int i = 0; i < 80; i++) {
        memory[FONT_START + i] = chip8_fontset[i];
    }
    randByte = std::uniform_int_distribution<unsigned char>(0, 255u);
}

void chip8::emulateCycle() {
    opcode = memory[pc] << 8 | memory[pc + 1];

    pc += 2;

    switch (opcode & 0xF000) {
    case 0x0000: {
        if (opcode == 0x00E0) {
            std::fill_n(display, sizeof(display) / sizeof(display[0]), 0);
        } else if (opcode == 0x00EE) {
            --sp;
            pc = stack[sp];
        }

    } break;
        // working
    case 0x1000: {
        pc = opcode & 0x0FFF;
    } break;

    case 0x2000: {
        stack[sp] = pc;
        sp++;
        pc = opcode & 0x0FFF;
    } break;
    case 0x3000: {
        if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
            pc += 2;
        }

    } break;
    case 0x4000: {
        if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
            pc += 2;
        }
    } break;
    case 0x5000: {
        if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
            pc += 2;
        }
    } break;
    case 0x6000: {
        V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
    } break;
    case 0x7000: {
        V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
    } break;
    case 0x8000: {
        unsigned char x = (opcode & 0x0F00) >> 8, y = (opcode & 0x00F0) >> 4;
        unsigned char condition = opcode & 0x000F;

        if (condition == 0x0) {
            V[x] = V[y];
        } else if (condition == 0x1) {
            V[x] = V[x] | V[y];
        } else if (condition == 0x2) {
            V[x] = V[x] & V[y];
        } else if (condition == 0x3) {
            V[x] = V[x] ^ V[y];
        } else if (condition == 0x4) {
            if (V[x] + V[y] > 255) {
                V[0xF] = 1;
            } else {
                V[0xF] = 0;
            }
            V[x] = (V[x] + V[y]) & 0xff;
        } else if (condition == 0x5) {
            if (V[x] > V[y]) {
                V[0xF] = 1;
            } else {
                V[0xF] = 0;
            }
            V[x] = V[x] - V[y];
        } else if (condition == 0x6) {
            if ((V[x] & 0x1) == 1) {
                V[0xf] = 1;
            } else {
                V[0xf] = 0;
            }
            V[x] = V[x] >> 1;
        } else if (condition == 0x7) {
            if (V[y] > V[x]) {
                V[0xF] = 1;
            } else {
                V[0xF] = 0;
            }
            V[x] = V[y] - V[x];
        } else if (condition == 0xE) {
            if ((V[x] & (1 << 7))) {
                V[0xF] = 1;
            } else {
                V[0xF] = 0;
            }
            V[x] = V[x] << 1;
        }

    } break;
    case 0x9000: {
        if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
            pc += 2;
        }
    } break;
    case 0xA000: {
        I = opcode & 0x0FFF;
    } break;
    case 0xb000: {
        pc = (opcode & 0x0FFF) + V[0x0];
    } break;
    case 0xc000: {
        auto random = randByte(randGen);
        V[(opcode & 0x0F00) >> 8] = random & (opcode & 0x00FF);
        pc += 2;
    } break;
    case 0xd000: {
        unsigned char poxx = V[(opcode & 0x0F00) >> 8];
        unsigned char posy = V[(opcode & 0x00F0) >> 4];
        unsigned short height = opcode & 0x000f;
        unsigned short pixel;

        unsigned char x = poxx % 64;
        unsigned char y = posy % 32;

        V[0xF] = 0;
        for (int col = 0; col < height; col++) {
            pixel = memory[I + col];
            for (int row = 0; row < 8; row++) {
                unsigned char pixelCondition = pixel & (0x80 >> row);
                uint32_t *exactPixel = &display[x + row + ((y + col) * 64)];
                if (pixelCondition) {
                    if (*exactPixel == 0xFFFFFFFF) {
                        V[0xf] = 1;
                    }
                    *exactPixel ^= 0xFFFFFFFF;
                }
            }
        }

    } break;
    case 0xe000: {
        auto x = (opcode & 0x0F00) >> 8;
        if ((opcode & 0x00ff) == 0x009E) {
            if (key[V[x]] == 1) {
                pc += 2;
            }
        } else if ((opcode & 0x00ff) == 0x00A1) {
            if (key[V[x]] != 1) {
                pc += 2;
            }
        }
    } break;
    case 0xf000: {
        unsigned char x = (opcode & 0x0F00) >> 8;
        unsigned char condition = opcode & 0x00FF;

        if (condition == 0x07) {
            V[x] = delay_timer;
        } else if (condition == 0x0A) {
            uint8_t x = (opcode & 0x0F00) >> 8;

            if (key[0]) {
                V[x] = 0;
            } else if (key[1]) {
                V[x] = 1;
            } else if (key[2]) {
                V[x] = 2;
            } else if (key[3]) {
                V[x] = 3;
            } else if (key[4]) {
                V[x] = 4;
            } else if (key[5]) {
                V[x] = 5;
            } else if (key[6]) {
                V[x] = 6;
            } else if (key[7]) {
                V[x] = 7;
            } else if (key[8]) {
                V[x] = 8;
            } else if (key[9]) {
                V[x] = 9;
            } else if (key[10]) {
                V[x] = 10;
            } else if (key[11]) {
                V[x] = 11;
            } else if (key[12]) {
                V[x] = 12;
            } else if (key[13]) {
                V[x] = 13;
            } else if (key[14]) {
                V[x] = 14;
            } else if (key[15]) {
                V[x] = 15;
            } else {
                pc -= 2;
            }

        } else if (condition == 0x15) {
            delay_timer = V[x];
        } else if (condition == 0x18) {
            sound_timer = V[x];
        } else if (condition == 0x1E) {
            I = I + V[x];
        } else if (condition == 0x29) {
            I = 0x50 + (5 * V[x]);
        } else if (condition == 0x33) {
            unsigned char rem;
            memory[I] = V[x] / 100;
            rem = V[x] % 100;
            memory[I + 1] = rem / 10;
            memory[I + 2] = rem % 10;
        } else if (condition == 0x55) {
            for (int i = 0; i <= x; i++) {
                memory[I + i] = V[i];
            }
        } else if (condition <= 0x65) {
            for (int i = 0; i < x; i++) {
                V[i] = memory[I + i];
            }
        }

    } break;

    default:
        cout << "Unknown opcode: 0x%X\n" << opcode << endl;
    }

    if (delay_timer > 0)
        --delay_timer;
    if (sound_timer > 0) {
        --sound_timer;
    }
}

const unsigned int START_ADDRESS = 0x200;

/// Loads the rom into memory 0x200 and quits
void chip8::LoadROM(char const *filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        std::streampos size = file.tellg();
        char *buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (long i = 0; i < size; ++i) {
            memory[START_ADDRESS + i] = buffer[i];
        }

        delete[] buffer;
    }
}
