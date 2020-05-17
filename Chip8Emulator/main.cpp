#include "Graphics.h"  // OpenGL and SDL graphics and input
#include "Input.h"
#include "chip8.h" // Your cpu core implementation
#include <windows.h>                // for Windows APIs
#include <chrono>
#include <numeric>


chip8 myChip8;
Graphics Gfx;
Input input;
gfx_state State = DEBUG;

void drawGraphics()
{
	static int numFrames = 0;
	static Uint32 startTime = SDL_GetTicks();

	static Uint32 lastFrame = startTime;

	Uint32 elapsedMS = SDL_GetTicks() - startTime; // Time since start of loop
	/*if (elapsedMS) { // Skip this the first frame
		double elapsedSeconds = elapsedMS / 1000.0; // Convert to seconds
		double fps = numFrames / elapsedSeconds; // FPS is Frames / Seconds
		//SDL_Delay(1000.0 / 60.0); // Use floating point division, not integer
	}*/

	if (SDL_GetTicks() < lastFrame + 1000.0f/60.0f )
	{
		return;
	}
	else
	{
		++numFrames;
		lastFrame = SDL_GetTicks();
		//cout << numFrames / (elapsedMS / 1000.0) << endl;
	}

	switch (State)
	{
	case DEBUG:
		for (int i = 0; i < 32; i++)
		{
			for (int j = 0; j < 64; j++)
			{
				if (myChip8.GetGfx()[i * 64 + j] == 0)
				{
					cout << " ";
				}
				else cout << "#";
			}
			cout << endl;
		}
		break;
	case SDL:
	{
		SDL_SetRenderDrawColor(Gfx.GetRenderer(), 0, 0, 0, 255);//black
		SDL_RenderClear(Gfx.GetRenderer());
		SDL_SetRenderDrawColor(Gfx.GetRenderer(), 255, 255, 255, 255);
		for (int i = 0; i < RES_Y; i++)
		{
			for (int j = 0; j < RES_X; j++)
			{
				if (myChip8.GetGfx()[i * RES_X + j] != 0)
				{
					Gfx.Draw_sprite(j*SCALE, i*SCALE);
				}
			}
		}
		SDL_RenderPresent(Gfx.GetRenderer());

		break;
	}
	default:
		break;
	}
}

int main(int argc, char **argv)
{
	// Set up render system and register input callbacks
	if(State != DEBUG) Gfx.SDL_Gfx_Init(); // Opengl/SDL part
	input.SDL_Event_Init();	//SDL part

	// Initialize the Chip8 system and load the game into the memory  
	myChip8.initialize();
	
	/*myChip8.Compile("TEST");
	myChip8.Dissasemble("TEST");
	*/
	
	myChip8.loadGame("c8games/PONG");

	int FPS = 50; //Framerate

	std::vector<double> emulate_times;

	// Emulation loop
	for (;;)// while(1) equivalent
	{
		auto start = chrono::steady_clock::now();
		
		// Emulate one cycle
		myChip8.emulateCycle();

		auto end = chrono::steady_clock::now();
		auto diff = end - start;
		emulate_times.push_back(chrono::duration <double, milli>(diff).count());

		if (emulate_times.size() == 150)
		{
			double sum = std::accumulate(emulate_times.begin(), emulate_times.end(), 0.0);
			double mean = sum / emulate_times.size();
			cout << " mean value : "  <<  mean << " ms" << endl;
			system("pause");
		}

		//cout << chrono::duration <double, milli>(diff).count() << " ms" << endl;
		// If the draw flag is set, update the screen
		if (myChip8.drawFlag)
			drawGraphics();//Opengl/SDl part

		// Store key press state (Press and Release)
		myChip8.setKeys();

		// update timers
		/*myChip8.Update_Timer();
		myChip8.Update_Sound();*/

		input.Event_handler(myChip8,Gfx);

		if (Gfx.Exit_App() == true)
			break;
		// execute at 60 hz
		Sleep(1000.0f/960.0f);
		
		
		

		/*if (100>(SDL_GetTicks() - startTime))
		{
			SDL_Delay((100/FPS) - (SDL_GetTicks() - startTime)); //SDL_Delay pauses the execution.
		}*/

		if(State == DEBUG)
			system("cls");

		/*cout << "Register Map" << endl;
		for (int i = 0; i < 16; i++)
		{
			printf("0x%X\n",myChip8.GetV()[i]);
		}
		system("cls");*/
	}

	return 0;
}