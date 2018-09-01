#pragma once

class chip8 {
	public:
		chip8();
		~chip8();
		bool drawFlag;

		unsigned char gfx[64 * 32];  //2042 pixels
		unsigned char key[16];
		void emulateCycle();
		bool loadApplication(const char *filename);

	private:
		unsigned short pc;      //Program Counter
		unsigned short opcode;  //current opcode
		unsigned short I;		//Index register
		unsigned short sp;		//Stack Pointer

		unsigned char V[16];	//V-regs (V0-VF)
		unsigned short stack[16];  //Stack (16 levels)
		unsigned char memory[4096];//Memory (size == 4k)

		unsigned char delay_timer;
		unsigned char sound_timer;

		void init();
};