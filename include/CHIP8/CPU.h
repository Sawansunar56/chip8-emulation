#ifndef _CPU_H_
#define _CPU_H_

#include <random>
class chip8 {
  private:
    unsigned short opcode;
    unsigned char memory[4096] = {0};
    unsigned char V[16] = {0};
    unsigned short I;
    unsigned short pc;

    unsigned char display[64 * 32] = {0};

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[16] = {0};
    unsigned short sp;

    unsigned char key[16];

  public:
    std::default_random_engine randGen;
    std::uniform_int_distribution<unsigned char> randByte;
    chip8();
    void emulateCycle();
    void LoadROM(char const* filename);
};


#endif // !_CPU_H_
