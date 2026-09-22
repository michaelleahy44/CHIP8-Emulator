#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

typedef struct
{
    unsigned short opcode; // 16-bit
    unsigned char memory[4096]; // 4096 bytes of memory
    unsigned char V[16]; // V0 to VF -> 16 8-bit registers
    unsigned short I; // 16-bit used to stroe memory addresses, only rightmost(lowest) 12 bits are usually used
    unsigned short pc; // 16-bit
    unsigned char gfx[SCREEN_WIDTH*SCREEN_HEIGHT];
    unsigned char delay_timer; // 8-bit
    unsigned char sound_timer; // 8-bit
    unsigned short stack[16]; // 16 16-bit values
    unsigned short sp; // 8-bit
    unsigned char key[16];
} Chip8;

void initialize(Chip8* chip);

int main(int argc, char* argv[])
{
    Chip8 chip;
    initialize(&chip);
    draw_screen();

    for(;;)
    {
        switch (chip.opcode)
        {
            // 0NNN
            case 0x00E0:
                // display -> clear the screen
                // disp_clear()
                break;
            case 0x00EE:
                // flow -> returns from a subroutine
                // return;
                break;
            // 1NNN
            // 2NNN
            // 3XNNN
            // 4XNN
            // 5XY0
            // 6XNN
            // 7XNN
            // 8XY0
            // 8XY1
            // 8XY2
            // 8XY3
            // 8XY4
            // 8XY5
            // 8XY6
            // 8XY7
            // 8XYE
            // 9XY0
            // ANNN
            // BNNN
            // CXNN
            // DXYN
            // EX9E
            // EXA1
            // FX07
            // FX15
            // FX18
            // FX1E
            // FX29
            // FX33
            // FX55
            // FX65
        }
    }

    return 0;
}

void initialize(Chip8* chip)
{
    // start at location 0x200 (512).
    // Everything below is reserved for interpreter
    chip->pc = 0x200;
    chip->sp = 0x200;
}