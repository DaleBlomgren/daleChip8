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
	for (int i = 0; i < 2048; ++i)
		gfx[i] = 0;
	// clear stack
	for (int i = 0; i < 16; ++i)
		stack[i] = 0;

	for (int i = 0; i < 16; ++i)
		key[i] = V[i] = 0;
	// clear memory
	for (int i = 0; i < 4096; ++i)
		memory[i] = 0;
	
	// Load fontset
	for (int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

	//reset timers
	delay_timer = 0;
	sound_timer = 0;

	//clear screen
	drawFlag = true;

	srand(time(NULL));
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
		case 0x1000:    // 1NNN: Jumps to address NNN
			pc = opcode & 0x0FFF;
		break;

		case 0x2000:	// 2NNN: Calls subroutine at NNN
			stack[sp] = pc;
			++sp;
			pc = opcode & 0x0FF;
		break;

		case 0x3000:	//3XNN: Skips the next instruction of VX doesn't equal NN
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
		break;

		case 0x4000:    //4XNN: Skips the next instruction if VX doesn't equal NN
			if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
		break;

		case 0x5000:	//5XY0: Skips the next instruction if VX equals VY
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
		break;

		case 0x6000:	//6XNN: Sets VX to NN
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			pc += 2;
		break;

		case 0x7000:	//7XNN: Adds NN to VX (Carry flag not changed)
			V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			pc += 2;
		break;

		case 0x8000:
			switch (opcode & 0x000F)
			{
				case 0x0000:	//8XY0: Sets VX to the value of VY
					V[(opcode & 0x00F0) >> 8] = V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0001:	//8XY1: Sets VX to "VX or VY"
					V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0002: // 0x8XY2: Sets VX to "VX and VY"
					V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0003:	//8XY3: Sets VX to "VX xor VY"
					V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0004:	//8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't. 
					if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
						V[0xF] = 1; //carry
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0005:	//8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't. 
					if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
						V[0xF] = 0; // there is a borrow
					else
						V[0xF] = 1;
					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0006:	//8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
					V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
					V[(opcode & 0x0F00) >> 8] >>= 1;
					pc += 2;
				break;

				case 0x0007:	//8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't. 
					if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])	// VY-VX
						V[0xF] = 0; // there is a borrow
					else
						V[0xF] = 1;
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
					pc += 2;
				break;

				case 0x000E: // 0x8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
					V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
					V[(opcode & 0x0F00) >> 8] <<= 1;
					pc += 2;
				break;

				default:
					printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
			}
		break;

		case 0x9000:	// 0x9XY0: Skips the next instruction if VX doesn't equal VY.
			if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
		break;

		case 0xA000:	//ANNN: Sets I to the address NNN
			I = opcode & 0x0FFF;
			pc += 2;
		break;

		case 0xB000:	///BNNN: Jumps to the address NNN plus V0
			pc = (opcode & 0x0FFF) + V[0];
		break;

		case 0xC000:	//CXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN. 

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
		
		case 0xF000:
			switch (opcode & 0x00FF) {
				case 0x0007:		// FX07: Sets VX to the value of the delay timer.
					V[(opcode & 0x0F00) >> 8] = delay_timer;
					pc += 2;
				break;

				case 0x000A: {		// FX0A: A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
					bool pressed = false;

					for (int i = 0; i < 16; ++i) {
						if (key[i] != 0) {
							V[(opcode & 0x0F00) >> 8] = i;
							pressed = true;
						}
					}
					//  skip a cycle and try again
					if (!pressed)
						return;
					pc += 2;
				}
				break;

				case 0x0015:		// FX15: Sets the delay timer to VX. 
					delay_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
				break;

				case 0x0018:		// FX18: Sets the sound timer to VX.
					sound_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
				break;

				case 0x001E:		// FX1E:	Adds VX to I
					I = V[(opcode & 0x0F00) >> 8];
					pc += 2;
				break;

				case 0x0029:		// FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font. 
					I = V[(opcode & 0x0F00) >> 8] * 0x5;
					pc += 2;
				break;

				case 0x0033:		//FX33: Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2.
									//In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
					memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;
				break;

				case 0x0055:		//FX55: Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
					for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
						memory[I + i] = V[i];

					I += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
				break;

				case 0x0065:		//FX65: Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified. 
					for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
						V[i] = memory[I + i];

					I += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
				break;

				default:
					printf("Unknown opcode [0xF000]: 0x%X\n", opcode);
			}

		break;

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

bool chip8::loadApplication(const char * filename)
{
	init();
	printf("Loading: %s\n", filename);

	// Open file
	FILE * pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		return false;
	}

	// Check file size
	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);
	printf("Filesize: %d\n", (int)lSize);

	// Allocate memory to contain the whole file
	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		return false;
	}

	// Copy buffer to Chip8 memory
	if ((4096 - 512) > lSize)
	{
		for (int i = 0; i < lSize; ++i)
			memory[i + 512] = buffer[i];
	}
	else
		printf("Error: ROM too big for memory");

	// Close file, free buffer
	fclose(pFile);
	free(buffer);

	return true;
}