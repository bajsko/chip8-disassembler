#include <stdio.h>
#include "cpu.h"

int main(int argc, char **argv)
{
	CHIP8* cpu = loadCpu("PONG");
	
	printf("\n\n ====DISASSEMBLE START====\n\n");
	
	while(cpu->pc < (cpu->d_file_size + 0x200))
	{
		disassemble(cpu);
	}
	
	printf("\n\n ====DISASSEMBLE END====\n\n");
	
	return 0;
}
