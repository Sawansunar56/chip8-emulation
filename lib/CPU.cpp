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
    opcode = 0;
    I = 0;
    sp = 0;

    // cout << "Memory"<< memory[0] << endl;
    // cout << "V" << V[0] << endl;
    // cout << "gfx" << gfx[0] << endl;
    //
    // cout << "stack" << stack[14] << endl;

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
        }
        if (opcode == 0x00EE) {
        }

    } break;
    case 0x1000: {
        pc = opcode & 0x0FFF;
    } break;
    case 0x2000: {
        sp++;
        stack[sp] = pc;
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

        if (condition == 0x2) {
            V[x] = V[x] & V[y];
        } else if (condition == 0x3) {
            V[x] = V[x] ^ V[y];

        } else if (condition == 0x4) {
            V[x] = V[x] ^ V[y];
        }

    } break;
    case 0x9000: {
    } break;
    case 0xA000: {
        I = opcode & 0x0FFF;
        pc += 2;
    } break;
    case 0xb000: {
    } break;
    case 0xc000: {
    } break;
    case 0xd000: {
    } break;
    case 0xf000: {
    } break;

    default:
        cout << "Unknown opcode: 0x%X\n" << opcode << endl;
    }

    if (delay_timer > 0)
        --delay_timer;
    if (sound_timer > 0) {
        if (sound_timer == 1) {
            cout << "BEEP!\n";
            --sound_timer;
        }
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
