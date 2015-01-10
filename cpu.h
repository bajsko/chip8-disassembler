#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct CHIP8
{
	unsigned char memory[4096];
	unsigned char V[16];
	unsigned char stack[16];
	unsigned char keyboard[16];
	unsigned char gfx[64*32];
	
	unsigned int pc;
	unsigned int I;
	unsigned int sp;
	
	unsigned char delay_timer;
	unsigned char sound_timer;
	
	//Debug only
	unsigned int d_file_size;

} CHIP8;

CHIP8* loadCpu(const char* string);
void reset(CHIP8* cpu);
void emulate(CHIP8* cpu);
void disassemble(CHIP8* cpu);