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

unsigned char chip8_fontset[] =
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

void initialize(Chip8* chip);
int loadProgram(char* name, Chip8 chip);
void emulateCycle(Chip8* chip);

int main(int argc, char* argv[])
{
    Chip8 chip;

    // Set up render system and register input callback
    setupGraphics();
    setupInput();

    // Initialize the Chip8 system and load the game into the memory
    initialize(&chip);
    if (loadProgram("pong", chip) == -1)
    {
        return -1;
    }

    // Emulation loop
    for(;;)
    {
        // emulate one cycle
        emulateCycle(&chip);

        // if the draw flag is set, update the screen
        // draw flag set by 0x00E0 and 0xDXYN

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

    // TODO: Clear display

    // Clear stack
    for (int i = 0; i < 16; i++)
        chip->stack[i] = 0;
    
    // Clear registers V0-VF
    for (int i = 0; i < 16; i++)
        chip->V[i] = 0;
    
    // Clear memory
    for (int i = 0; i < 4096; i++)
        chip->memory[i] = 0;

    // Load fontset
    for (int i = 0; i < 80; i++) {
        chip->memory[i] = chip8_fontset[i];
    }

    // Reset timers
    // TODO: figure out what starting value should be
    chip->delay_timer = 60;
    chip->sound_timer = 60;
}

int loadProgram(char* name, Chip8* chip) {
    // use fopen in binary mode to open program
    // start filling memory at location 0x200 = 512
    FILE *fptr = NULL;
    int bufferSize = 4096;
    char buffer[bufferSize];
    fptr = fopen(name, "rb");
    if (fptr == NULL)
    {
        printf("Failed to open file %s\n", name);
        return -1;
    }
    fgets(buffer, bufferSize, fptr);
    for (int i = 0; i < bufferSize; i++)
        chip->memory[i + 512] = buffer[i];

    fclose(fptr);
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
                // 0x0NNN
                default:
                    printf("Unknown opcode: 0x%X\n", chip->opcode);
            }
            break;
        case 0x1000: // 0x1NNN
            // jump to address NNN
            break;
        case 0x2000: // 0x2NNN: calls subroutine at address NNN
            chip->stack[chip->sp] = chip->pc;
            ++chip->sp;
            chip->pc = chip->opcode & 0x0FFF;
            break;
        case 0x3000: // 0x3XNN
            // skip the next instruction if VX == NN
            break;
        case 0x4000: // 0x4XNN
            // skip next instruction if VX != NN
            break;
        case 0x5000: // 0x5XY0
            // skip next instruction if VX == VY
            break;
        case 0x6000: // 0x6XNN
            // set VX to NN
            break;
        case 0x7000: // 0x7XNN
            // Add NN to VX (carry flag not changed)
            break;
        case 0x8000:
            switch (chip->opcode & 0x000F)
            {
                case 0x0:
                    break;
                case 0x1:
                    break;
                case 0x2:
                    break;
                case 0x3:
                    break;
                case 0x4:
                    break;
                case 0x5:
                    break;
                case 0x6:
                    break;
                case 0x7:
                    break;
                case 0xE:
                    break;

                default:
                    printf("Unknown opcode: 0x%X\n", chip->opcode);
            }
            break;
        case 0x9000: // 0x9XY0
            // skip next instruction if VX !- VY
            break;
        case 0xA000: // ANNN: Sets I to the address NNN
            // Exectue opcode
            chip->I = chip->opcode & 0x0FFF;
            chip->pc += 2;
            break;
        case 0xB000: // 0xBNNN
            // jump to address NNNN + V0
            break;
        case 0xC000: // 0xCXNN
            // set VX to the result of bitwise and operation on a random number and NNN
            // VX = rand() & NN
            break;
        case 0xD000: // 0xDYN
            // draw sprite at coordinate (Vx, Vy) with width and height of N pixels
            break;
        case 0xE000:
            switch (chip->opcode & 0x00FF)
            {
                case 0x9E: // 0xEX9E
                    // skip next instruction if key in Vx is pressed
                    break;
                case 0xA1: // EXA1
                    // skip next instruction if key in Vx is not pressed
                    break;
                
                default:
                    printf("Unkown opcode: 0x%X\n", chip->opcode);
            }
        case 0xF000:
            switch (chip->opcode & 0x00FF)
            {
                case 0x07:
                    break;
                case 0x0A:
                    break;
                case 0x15:
                    break;
                case 0x18:
                    break;
                case 0x1E:
                    break;
                case 0x29:
                    break;
                case 0x33:
                    break;
                case 0x55:
                    break;
                case 0x65:
                    break;

                default:
                    printf("Unknown opcode: 0x%X\n", chip->opcode);
            }

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