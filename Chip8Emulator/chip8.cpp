#include "chip8.h"
#include <chrono>

chip8::chip8()
{
}

chip8::~chip8()
{
}

void chip8::initialize()
{
	// Initialize registers and memory once
	pc = 0x200;  // Program counter starts at 0x200
	opcode = 0;      // Reset current opcode	
	I = 0;		// Reset index register
	sp = 0;      // Reset stack pointer

				 // Clear display	
				 // Clear stack
				 // Clear registers V0-VF
				 // Clear memory

				 // Load fontset
	for (int i = 0; i < MEM_SIZE; i++)
	{
		memory[i] = 0;
	}

	for (int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

	// Reset timers
}

void chip8::emulateCycle()
{
	// Fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	//printf("0x%X\n", opcode);

	//cout << pc_array.size() << endl;

	pc_array.push_back(pc);
	opcodes_array.push_back(opcode);

	// reference for opcodes : http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00E0
	// Decode opcode
	switch (opcode & 0xF000)//get first number from opcode -> differenciation of cases and binary operations. 
	{
	case 0x0000://0x00XX
		switch (opcode & 0x000F)
		{
		case 0x0000: // 0x00E0: Clears the screen
			for (size_t i = 0; i < 64 * 32; i++)
			{
				gfx[i] = 0x0; // set all pixel to 0 (black)
				drawFlag = false;
			}

			pc += 2;
			break;
		case 0x000E://return from a subroutine
			//The interpreter sets the program counter to the address at the top of the stack, 
			//then subtracts 1 from the stack pointer.
			pc = stack[sp];
			sp--;
			pc += 2;
			break;
		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			break;
		}
		break;
	case 0x1000://JP addr
		//The interpreter sets the program counter to nnn.
		pc = opcode & 0x0FFF;
		break;
	case 0x2000://CALL addr
		/*The interpreter increments the stack pointer, 
		then puts the current PC on the top of the stack. The PC is then set to nnn.
		*/
		sp++;

		stack[sp] = pc;
		
		pc = opcode & 0x0FFF;
		break;
	case 0x3000://0x3XNN
		if (V[(opcode & 0x0F00) >> 8] == ((opcode & 0x00F0) | (opcode & 0x000F)))//if VX == NN, pc + 2
			pc += 2;// skip next instruction 
		pc += 2;// next instruction
		break;
	case 0x4000:
		if (V[(opcode & 0x0F00) >> 8] != ((opcode & 0x00F0) | (opcode & 0x000F)))//if VX != NN, pc + 2
			pc += 2;
		pc += 2;
		break;
	case 0x5000://if VX == VY, pc +2
		if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
			pc += 2;
		pc += 2;
		break;
	case 0x6000://VX = NN
		V[(opcode & 0x0F00) >> 8] = ((opcode & 0x00F0) | (opcode & 0x000F));
		pc += 2;
		break;
	case 0x7000:
		V[(opcode & 0x0F00) >> 8] += ((opcode & 0x00F0) | (opcode & 0x000F));
		pc += 2;
		break;
	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000://Vx = Vy LD
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x0001://Vx = Vx|Vy OR
			V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x0002://Vx = Vx&Vy
			V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x0003://Vx = Vx^Vy
			V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x0004://ADD Vx, Vy
			/*The values of Vx and Vy are added together.
			/ If the result is greater than 8 bits (i.e., > 255,)
			VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
			*/
			if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
				V[0xF] = 1; //carry
			else
				V[0xF] = 0;
			V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x0005:// SUB Vx, Vy
			/*
			If Vx > Vy, then VF is set to 1, otherwise 0.
			Then Vy is subtracted from Vx, and the results stored in Vx.
			*/
			if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
			{
				V[0xF] = 1;
			}
			else V[0xF] = 0;
			V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x0006:
			/*
			If the least-significant bit of Vx is 1, then VF is set to 1,
			otherwise 0. Then Vx is divided by 2.

			SHR is set to choose between the to modes
			*/
			if (SHR == 0)// Vx = Vx >> 1
			{
				if ((V[(opcode & 0x0F00) >> 8] & 0x1) == 0x1)
				{
					V[0xF] = 1;
				}
				else V[0xF] = 0;
				V[(opcode & 0x0F00) >> 8] >>= 1;
			}
			else if (SHR == 1)//Vx = Vy >> 1
			{
				if ((V[(opcode & 0x00F0) >> 4] & 0x1) == 0x1)
				{
					V[0xF] = 1;
				}
				else V[0xF] = 0;
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] >> 1;
			}
			pc += 2;
			break;
		case 0x0007://SUBN Vx, Vy
			/*
			If Vy > Vx, then VF is set to 1, otherwise 0.
			Then Vx is subtracted from Vy, and the results stored in Vx.
			*/
			if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
			{
				V[0xF] = 1;
			}
			else V[0xF] = 0;
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x000E://SHL Vx {, Vy}
			/*
			If the most-significant bit of Vx is 1,
			then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
			*/
			if ((V[(opcode & 0x0F00) >> 8]) >> 7 == 0x1)
			{
				V[0xF] = 1;
			}
			else V[0xF] = 0;
			V[(opcode & 0x0F00) >> 8] <<= 1;// Vx * 2
			pc += 2;
			break;
		default:
			printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
			break;
		}
		break;
	case 0x9000://SNE Vx, Vy
		//Skip next instruction if Vx != Vy.
		if ((V[opcode & 0x0F00] >> 8) != (V[opcode & 0x00F0] >> 4))
			pc += 2;
		pc += 2;
		break;
	case 0xA000: //LD I, addr
		/*ANNN: Sets I to the address NNN
		  Execute opcode
		*/
		I = opcode & 0x0FFF;
		pc += 2;
		break;
	case 0xB000://JP V0, addr
		pc = opcode & 0x0FFF + V[0x0];
		break;
	case 0xC000:
		/*
		The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk.
		The results are stored in Vx. See instruction 8xy2 for more information on AND.
		*/
		V[(opcode & 0x0F00) >> 8] = rand() % 255 & (opcode & 0x0F00 | opcode & 0x000F);
		pc += 2;
		break;
	case 0xD000:
	{
		// shift from 8 bits right
		unsigned short x = V[(opcode & 0x0F00) >> 8];
		unsigned short y = V[(opcode & 0x00F0) >> 4];
		unsigned short height = opcode & 0x000F;
		unsigned short pixel;

		// set VF at 0 (no collision)
		V[0xF] = 0;
		for (int yline = 0; yline < height; yline++)//each row
		{
			pixel = memory[I + yline];
			for (int xline = 0; xline < 8; xline++)//loop over 8 bits of one row
			{
				if ((pixel & (0x80 >> xline)) != 0)//check if current pixel is set to 1
				{
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
		switch (opcode & 0x00FF)
		{
			// EX9E: Skips the next instruction 
			// if the key stored in VX is pressed
		case 0x009E:
			if (key[V[(opcode & 0x0F00) >> 8]] != 0)
				pc += 4;
			else
				pc += 2;
			break;
		case 0x00A1:
			if (key[V[(opcode & 0x0F00) >> 8]] != 1)
				pc += 4;
			else
				pc += 2;
			break;
		default:
			printf("Unknown opcode 0xE000 : 0x%X\n", opcode);
			break;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:// LD Vx, DT
			V[(opcode & 0x0F00) >> 8] = delay_timer;
			pc += 2;
			break;
		case 0x000A:
		{
			//bool ispressed = false;
			//All execution stops until a key is pressed, then the value of that key is stored in Vx.
			//while (!ispressed)
			//{
				for (int i = 0; i < 16; i++)
				{
					if (key[i] == 1)// key pressed
					{
						V[(opcode & 0x0F00) >> 8] = i;
						pc += 2;
						//ispressed = true;
					}
					else pc -= pc;// if no key pressed we replay the same instruction
				}
			//}
			break;
		}
		case 0x0015:
			delay_timer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x0018:
			sound_timer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x001E:
			I = I + V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x0029:
			/*
			The value of I is set to the location for the hexadecimal sprite 
			corresponding to the value of Vx.
			*/
			I = V[(opcode & 0x0F00) >> 8]*5;//index in font set equal to Vx
			pc += 2;
			break;
		case 0x0033:
			memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
			memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
			memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
			pc += 2;
			break;
		case 0x0055:
			/*The interpreter copies the values of registers V0 through Vx into memory, 
			starting at the address in I.
			*/
			for (int i = 0; i <= (int)(opcode & 0x0F00) >> 8; i++)
			{
				memory[I + i] = V[i];
			}
			pc += 2;
			break;
		case 0x0065:
			/*
			The interpreter reads values from memory starting at location I into registers V0 through Vx.
			*/
			for (int i = 0; i <= (int)(opcode & 0x0F00) >> 8; i++)
			{
				 V[i] = memory[I + i];
			}
			pc += 2;
			break;
		default:
			printf("Unknown opcode 0xF000 : 0x%X\n", opcode);
			break;
		}
		break;
	default:
		printf("Unknown opcode: 0x%X\n", opcode);
		break;
	}

	// Update timers
	if (delay_timer > 0)
		--delay_timer;

	if (sound_timer > 0)
	{
		if (sound_timer == 1)
			printf("BEEP!\n");
		--sound_timer;
	}

}

// load game data from file and store in memory at 0x200 (512) 
void chip8::loadGame(char * game_file)
{
	int c = 0;
	int bufferSize = 0;
	FILE * pFile = NULL;
	vector<char> buffer;

	// fopen binary mode
	pFile = fopen(game_file, "rb");

	if (!pFile)
	{
		printf("Error, game file could not be loaded !\n");
	}

	while (!feof(pFile)) {
		c = fgetc(pFile);
		bufferSize++;
		buffer.push_back(c);
		//printf("0x % 02X\n", c);
	}
	fclose(pFile);
	
	for (int i = 0; i < bufferSize; ++i)
		memory[i + 0x200] = buffer.at(i);
}

void chip8::setKeys()
{
	for (int i = 0; i < 16; i++)
	{
		if (key[i] == 1)
			break;
	}
}

void chip8::setKey(unsigned char key_num,int val)
{
	key[key_num] = val;
}

unsigned char * chip8::GetV()
{
	return V;
}

void chip8::Update_Timer()
{
	if (delay_timer>0) delay_timer -= 1;
}

void chip8::Update_Sound()
{
	if (sound_timer>0)sound_timer -= 1;
}


void chip8::Dissasemble(char * file_name)
{
	FILE * pFile = NULL;
	// fopen binary mode
	pFile = fopen(strcat(file_name,".ch8"), "rw");

	if (!pFile)
	{
		printf("Error, asm file could not be created !\n");
	}

	// reference for opcodes : http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00E0
	// Decode opcode
	switch (opcode & 0xF000)//get first number from opcode -> differenciation of cases and binary operations. 
	{
	case 0x0000://0x00XX
		switch (opcode & 0x000F)
		{
		case 0x0000: // 0x00E0: Clears the screen        
			fputs("CLS\n", pFile);
			break;
		case 0x000E://return from a subroutine
			fputs("RET\n", pFile);
			break;
		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
		}
		break;
	case 0x1000://JP addr
	{
		char buffer[20] = "";
		sprintf(buffer, "%d", (opcode & 0x0FFF));
		fputs(strcat("JP", buffer), pFile);
		fputs("\n", pFile);
	}
		break;
	case 0x2000://CALL addr
		break;
	case 0x3000://0x3XNN

		break;
	case 0x4000:
		
		break;
	case 0x5000://if VX == VY, pc +2
		
		break;
	case 0x6000://VX = NN
		
		break;
	case 0x7000:
		
		break;
	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000://Vx = Vy LD
			
			break;
		case 0x0001://Vx = Vx|Vy OR
			
			break;
		case 0x0002://Vx = Vx&Vy
			
			break;
		case 0x0003://Vx = Vx^Vy
			
			break;
		case 0x0004://ADD Vx, Vy
				
			break;
		case 0x0005:// SUB Vx, Vy
				
			break;
		case 0x0006:// SHR
			
			break;
		case 0x0007://SUBN Vx, Vy
				
			break;
		case 0x000E://SHL Vx {, Vy}
		
			break;
		default:
			printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
			break;
		}
		break;
	case 0x9000://SNE Vx, Vy
		
		break;
	case 0xA000: //LD I, addr
	
		break;
	case 0xB000://JP V0, addr
		
		break;
	case 0xC000:
	
		break;
	case 0xD000:
	{
		
	}
	break;
	case 0xE000:
		switch (opcode & 0x00FF)
		{
			// EX9E: Skips the next instruction 
			// if the key stored in VX is pressed
		case 0x009E:
		
			break;
		case 0x00A1:
			
			break;
		default:
			printf("Unknown opcode 0xE000 : 0x%X\n", opcode);
			break;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:// LD Vx, DT
		
			break;
		case 0x000A:
		{
			
			break;
		}
		case 0x0015:
			
			break;
		case 0x0018:
			
			break;
		case 0x001E:
			
			break;
		case 0x0029:

			break;
		case 0x0033:
		
			break;
		case 0x0055:
			
			break;
		case 0x0065:
		
			break;
		default:
			printf("Unknown opcode 0xF000 : 0x%X\n", opcode);
			break;
		}
		break;
	default:
		printf("Unknown opcode: 0x%X\n", opcode);
		break;
	}
}

void chip8::Compile(char * file_name)
{
	FILE * pFile = NULL;
	FILE * dest = NULL;

	// fopen binary mode
	string string_name(file_name);
	string dest_name = string_name + ".ch8";

	string_name = string_name + ".asm";
	pFile = fopen(string_name.c_str(), "r");
	
	dest = fopen(dest_name.c_str(), "wb");

	if (!pFile)
	{
		printf("Error, ch8 file could not be created !\n");
	}

	char buffer[20] = "";
	
	//fgets(buffer, 50, pFile);

	string buffer_str;
	stringstream ss(buffer_str);

	while (fgets(buffer, 50, pFile) != NULL)
	{
		buffer_str = string(buffer);

		if (buffer_str[0] == 'J')// if first instruction is jump
		{
			WriteOpcode(buffer_str, dest, 0x1000);
		}
		if (buffer_str.at(0) == 'R')//if return
		{
			WriteOpcode(buffer_str, dest, 0x00E0);
		}
		if (buffer_str.at(0) == 'C')// if clear 
		{
			WriteOpcode(buffer_str, dest, 0x00EE);
		}

	}

	fclose(dest);
	fclose(pFile);
}

void chip8::WriteOpcode(string buffer_str, FILE * dest, int code)
{
	string instruction = "";

	instruction = buffer_str.substr(buffer_str.find("x") + 1);// find adress value after x in asm code to catch adress
	int i = (code | (int)strtol(instruction.c_str(), NULL, 16));// merge hex value code with adress to get opcode value 
	char* c = (char*)&i;// convert to char 
	// write opcode in inverted order because of char convertion (?)
	fwrite(&c[1], 1, 1, dest);
	fwrite(&c[0], 1, 1, dest);
}
