#include "cpu.h"

CHIP8* loadCpu(const char* string)
{
	FILE *fp = fopen(string, "rb");
	
	if(fp == NULL) 
	{
		printf("Couldn't open file, exiting"); 
		return NULL;
	}
	
	printf("Loading ROM %s, please wait..\n", string);
	
	CHIP8 *cpu = calloc(sizeof(CHIP8), 1);
	reset(cpu);
	
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);
	
	cpu->d_file_size = (int)size;
	
	printf("Filesize: %d bytes\n", (int)size);
	
	char* data = (char*)malloc(size);
	if(data == NULL)
	{
		printf("Couldn't create data buffer (Memory error)\n");
		return NULL;
	}
	
	fread(data, 1, size, fp);
	
	if((4096 - 0x200) > size)
	{
		for(int i = 0; i < size; i++)
		{
			cpu->memory[i + 0x200] = data[i];
		}
	} else
	{
		printf("ROM too big! (larger than 4kB)\n");
		return NULL;
	}
	
	fclose(fp);
	free(data);
	
	printf("ROM Loaded to CPU\n");
	
	return cpu;

}

void reset(CHIP8 *cpu)
{
	for(int i = 0; i < 4096; i++)
		cpu->memory[i] = 0;
	for(int i = 0; i < 16; i++)
		cpu->V[i] = 0;
	for(int i = 0; i < 16; i++)
		cpu->keyboard[i] = 0;
	for(int i = 0; i < 16; i++)
		cpu->stack[i] = 0;
	for(int i = 0; i < 64*32; i++)
		cpu->gfx[i] = 0;
		
	cpu->pc = 0x200;
	cpu->sp = 0;
	cpu->I = 0;
}

void emulate(CHIP8* cpu)
{
	short opcode = (cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc+1]);

}

void disassemble(CHIP8 * cpu)
{
	short opcode = (cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc+1]);
	
	printf("OPCODE 0x%X: ", (opcode & 0xF000) >> 12);
	
	unsigned short adress = (opcode & 0x0FFF);
	unsigned char X = (opcode & 0x0F00) >> 8;
	unsigned char Y = (opcode & 0x00F0) >> 4;
	
	unsigned char eightBitConst = (opcode & 0x00FF);
	unsigned char fourBitConst = (opcode & 0x000F);
	
	switch(opcode & 0xF000)
	{
		case 0x0000:
			switch(opcode & 0x000F)
			{
				case 0x0000:
					printf("CLR SCRN\n");
				break;
				
				case 0x000E:
					printf("RTRN SBRTN\n");
				break;
				
				default:
					printf("Unknown OPCODE [0x0000]: 0x%X\n", opcode);
				break;
			}
		break;
		
		case 0x1000:
			printf("JMP ADR: %03X\n", adress);
		break;
		
		case 0x2000:
			printf("CALL SBRTN: %03X\n", adress);
		break;
		
		case 0x3000:
			printf("SKIP IF EQ.: V%X, %02X\n", X, eightBitConst);
		break;
		
		case 0x4000:
			printf("SKIP IF N.EQ.: V%X, %02X\n", X, eightBitConst);
		break;
		
		case 0x5000:
			printf("SKIP IF EQ.: V%X, V%X\n", X, Y);
		break;
		
		case 0x6000:
			printf("SET V%X TO: %02X\n", X, eightBitConst);
		break;
		
		case 0x7000:
			printf("ADD %02X TO V%X\n", eightBitConst, X);
		break;
		
		case 0x8000:
			switch((opcode & 0x000F))
			{
				case 0x0000:
					printf("SET V%X TO VALUE OF V%X\n", X, Y);
				break;
				
				case 0x0001:
					printf("SET VX TO V%X or V%X\n", X, Y);
				break;
				
				case 0x0002:
					printf("SET VX TO V%X and V%X\n", X, Y);
				break;
				
				case 0x0003:
					printf("SET VX TO V%X xor V%X\n", X, Y);
				break;
				
				case 0x0004:
					printf("Add V%X to V%X\n", Y, X);
				break;
				
				case 0x0005:
					printf("SUB V%X FROM V%X\n", Y, X);
				break;
				
				case 0x0006:
					printf("SHIFT RIGHT 1 V%X\n", X);
				break;
				
				case 0x0007:
					printf("SET V%X TO V%X MINUS V%X\n", X, Y, X);
				break;
				
				case 0x000E:
					printf("SHIFT LEFT 1 V%X\n", X);
				break;
				
				default:
					printf("Unknown OPCODE [0x8000]: 0x%X\n", opcode);
				break;
			}
		break;
		
		case 0x9000:
			printf("SKIP IF NOT EQ.: V%X V%X\n", X, Y);
		break;
		
		case 0xA000:
			printf("SETS I TO ADRESS: %03X\n", adress);
		break;
		
		case 0xB000:
			printf("JUMP TO ADR: %03X PLUS V%X\n", adress, X);
		break;
		
		case 0xC000:
			printf("SET VX TO RAND and %02X\n", eightBitConst);
		break;
		
		case 0xD000:
			printf("SPRITE DRAWING\n");
		break;
		
		case 0xE000:
			switch((opcode & 0x00F0))
			{
				case 0x0090:
					printf("SKIP NEXT IF KEY IN V%X PRESSED\n", X);
				break;
				
				case 0x00A0:
					printf("SKIP NEXT IF KEY IN V%X NOT PRESSED\n", X);
				break;
				
				default:
					printf("Unknown OPCODE [0xE000]: 0x%X\n", opcode);
				break;
			}
		break;
		
		case 0xF000:
			switch((opcode & 0x00FF))
			{
				case 0x0007:
					printf("SET V%X TO VAL OF DELAY TIMER\n", X);
				break;
				
				case 0x000A:
					printf("WAIT KEYPRESS THEN STORE IN V%X\n", X);
				break;
				
				case 0x0015:
					printf("SET DELAY TIMER TO V%X\n", X);
				break;
				
				case 0x0018:
					printf("SET SOUND TIMER TO V%X\n", X);
				break;
				
				case 0x001E:
					printf("ADD V%X TO I\n", X);
				break;
				
				case 0x0029:
					printf("SET I TO LOC OF SPRITE IN V%X\n", X);
				break;
				
				case 0x0033:
					printf("STORE BINARY DECIMAL OF V%X\n", X);
				break;
				
				case 0x0055:
					printf("STORE V0 TO V%X IN MEM STARTING AT I\n", X);
				break;
				
				case 0x0065:
					printf("FILLS V0 TO V%X WITH VALUES FROM MEM STARTING AT I\n", X);
				break;
				
				default:
					printf("Unknown OPCODE [0xF000]: 0x%X\n", opcode);
				break;
			}
		break;
			
		default:
			printf("UNKWN OPCODE \n");
		break;
	}
	
	cpu->pc +=2;

}