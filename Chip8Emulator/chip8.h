#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <string>


#define SHR 0 //8xy6 - SHR Vx {, Vy}; 0 = Vx, 1 = Vy
#define MEM_SIZE 4096 // memory size 

#define RES_X 64
#define RES_Y 32
using namespace std;

enum gfx_state { DEBUG, SDL, OPENGL };// 

//http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/

class chip8 
{
	unsigned short opcode; // operation code, 2 bytes long

	unsigned char memory[MEM_SIZE]; // memory array of chip 8 

	unsigned char V[16]; // array of registers, 8 bits each

	unsigned short I;// index register
	unsigned short pc;//programm counter

	//0x000 - 0x1FF - Chip 8 interpreter(contains font set in emu)
	//0x050 - 0x0A0 - Used for the built in 4x5 pixel font set(0 - F)
	//0x200 - 0xFFF - Program ROM and work RAM

	// array used to store graphics, there is 2048 pixels to draw (64*32 res) we store pixel state (0 or 1, on or off)
	unsigned char gfx[RES_X * RES_Y];

	// 2 timers of chip 8, when count above 0, down to zero (modulo)
	unsigned int delay_timer;
	unsigned int sound_timer;

	// stack array for instructions, stack pointer to know state level 
	unsigned short stack[16];
	unsigned short sp;

	//HEX key array (0x0 - 0xF)
	// pressed 1, up 0
	unsigned char key[16];

	//DEBUG 

	std::vector<unsigned short> pc_array;
	std::vector<unsigned short> opcodes_array;

	// fontset for system 
	unsigned char chip8_fontset[80] =
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
	
public:
	chip8();
	~chip8();
	void initialize();
	void emulateCycle();
	void loadGame(char * file_name);
	void setKeys();
	void setKey(unsigned char key_num, int val);
	bool drawFlag;

	// Dissasemble chip8 binaries into assembly.
	void Dissasemble(char * file_name);
	// Compile chipe 8 assembly into chip8 binaries
	void Compile(char * file_name);
	void WriteOpcode(string buffer_str, FILE * dest, int code);

	unsigned char * GetGfx(){return gfx;}
	unsigned char * GetV();
	void Update_Timer();
	void Update_Sound();

	unsigned char GetTimer() { return delay_timer;}
	void SetTimer(unsigned char p_timer) { delay_timer = p_timer; }
	unsigned char GetSound() { return sound_timer; }
	void SetDound(unsigned char p_timer) { sound_timer = p_timer; }
};
