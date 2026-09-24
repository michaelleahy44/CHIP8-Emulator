#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

typedef struct chip8
{
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;
    unsigned char gfx[64 * 32];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char key[16];
} Chip8;

void initialize(Chip8* chip);
void loadProgram(char* name);
void emulateCycle(Chip8* chip);

int main(int argc, char* argv[])
{
    Chip8 chip;

    // Set up render system and register input callback
    setupGraphics();
    setupInput();

    // Initialize the Chip8 system and load the game into the memory
    initialize(&chip);
    loadProgram("pong");

    // Emulation loop
    for(;;)
    {
        // emulate one cycle
        emulateCycle(&chip);

        // if the draw flag is set, update the screen
        // draw flag set by 0x00E0 and 0xDXYN
        if(chip.draw_flag)
            drawGraphics();

        // store key press state (Press and Release)
        setKeys();
    }

    return 0;
}

void initialize(Chip8* chip)
{
    chip->pc = 0x200;
    chip->opcode = 0;
    chip->I = 0;
    chip->sp = 0;

    // Clear display
    // Clear stack
    // Clear registers V0-VF
    // Clear memory

    // Load fontset
    for (int i = 0; i < 80; i++) {
        chip->memory[i] = chip8_fontset[i];
    }

    // Reset timers
}

void loadProgram(char* name) {
    // use fopen in binary mode to open program
    // start filling memory at location 0x200 = 512
}

void emulateCycle(Chip8* chip)
{
    // Fetch Opcode
    chip->opcode = chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1];

    // Decode Opcode
    switch (chip->opcode & 0xF000)
    {
        case 0x0000:
            switch (chip->opcode & 0x000F)
            {
                case 0x0000: // 0x00E0: Clears the screen
                    // Execute opcode
                    break;
                case 0x000E: // 0x00EE: Returns from subroutine
                    // Execute opcode
                    break;
                
                default:
                    printf("Unknown opcode: 0x%X\n", chip->opcode);
            }
        case 0x0004: // 0x8XY4
            if (chip->V[(chip->opcode & 0x00F0) >> 4] > (0xFF - chip->V[(chip->opcode & 0x0F00) >> 8]))
                chip->V[0xF] = 1; // carry
            else
                chip->V[0xF] = 0;
            chip->V[(chip->opcode & 0x0F00) >> 8] += chip->V[(chip->opcode & 0x00F0) >> 4];
            chip->pc += 2;
            break;
        case 0x2000: // 0x2NNN: calls subroutine at address NNN
            chip->stack[chip->sp] = chip->pc;
            ++chip->sp;
            chip->pc = chip->opcode & 0x0FFF;
            break;
        case 0xA000: // ANNN: Sets I to the address NNN
            // Exectue opcode
            chip->I = chip->opcode & 0x0FFF;
            chip->pc += 2;
            break;
        case 0x0033: // 0xFX33
            chip->memory[chip->I] = chip->V[(chip->opcode & 0x0F00) >> 8] / 100;
            chip->memory[chip->I + 1] = (chip->V[(chip->opcode & 0x0F00) >> 8] / 10) % 10;
            chip->memory[chip->I + 2] = (chip->V[(chip->opcode & 0x0F00) >> 8] % 100) % 1;
            chip->pc += 2;
            break;

        default:
            printf("Unknown opcode: 0x%X\n", chip->opcode);
    }

    // Update timers
    if (chip->delay_timer > 0)
        --chip->delay_timer;

    if (chip->sound_timer > 0)
    {
        if (chip->sound_timer == 1)
            printf("BEEP!\n");
        --chip->sound_timer;
    }
}