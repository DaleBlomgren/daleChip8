#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

chip8::chip8()
{
	
}

chip8::~chip8() {

}

unsigned char chip8_fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void chip8::init()
{
	pc = 0x200;  // Starting address for program
	opcode = 0;  // Current opcode
	I = 0;		 // Current index register
	sp = 0;      // Current stack pointer

	// clear display
	// clear stack
	// clear registers (V0-VF)
	// clear memory

	// Load fontset


	//reset timers
}

void chip8::emulateCycle()
{
	//fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1]; //bitwise or operation 
	//decode opcode
	switch (opcode & 0xF000) {
		//opcodes//
		case 0x0000:
			switch (opcode & 0x000F)
			{
				case 0x0000:	// 0x00E0: clear screen
					for (int i = 0; i < 2048; ++i)
						gfx[i] = 0x0;
					drawFlag = true;
					pc += 2;
				break;

				case 0x000E:		//0x00EE: returns from subroutine
					--sp;			// decrement the level of your stack
					pc = stack[sp]; //insert stored return address from stack back to your program counter
					pc += 2;
				break;

				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			}
		case 0xA000:	//ANNN: Sets I to the address NNN
			//execute opcode
			I = opcode & 0x0FFF;
			pc += 2;
		break;

		case 0xD000: //graphicx
		{
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short height = opcode & 0x00F;
			unsigned short pixel;

			V[0xF] = 0;
			for (int yline = 0; yline < height; yline++) {
				pixel = memory[I + yline];
				for (int xline = 0; xline < 8; xline++) {
					if ((pixel & (0x80 >> xline)) != 0) {
						if (gfx[(x + xline + ((y + yline) * 64))] == 1)
							V[0xF] = 1;
						gfx[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}

			drawFlag = true;
			pc += 2;
		}
		break;

		case 0xE000:			
			switch (opcode & 0x00FF) {
				case 0x009E:				// EX9E: Skips the next instruction if key stroed in VX is pressed
					if (key[V[opcode & 0x0F00]] != 0)
						pc += 4;
					else
						pc += 2;
				break;

				case 0x00A1:				//EXA1: Skips next instruction if the key stored in VX ISN'T pressed
					if (key[V[(opcode & 0x0F00) >> 8]] == 0)
						pc += 4;
					else
						pc += 2;
				break;

				default:
					printf("Unknown opcode[0xE000]: 0x%X\n", opcode);
			}
			break;
		//more opcodes//

		default:
			printf("Unknown opcode: 0x%X\n", opcode);
	}
	

	//update timers

	if (delay_timer > 0)
		--delay_timer;

	if (sound_timer > 0) {
		if (sound_timer == 1)
			printf("BEEP!\n");
		--sound_timer;
	}
}