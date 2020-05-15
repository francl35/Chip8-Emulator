#include "chip8.h" // Your cpu core implementation
// #include "Graphics.h"  // OpenGL and SDL graphics and input
#include "Input.h"
#include "common.h"
// #include <windows.h>                // for Windows APIs

PSP_MODULE_INFO("HELLO WORLD", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

SceCtrlData padData;
SceCtrlLatch latch;

chip8 myChip8;
Graphics Gfx;
Input input;
gfx_state State = SDL;

void drawGraphics()
{
	static int numFrames = 0;
	static Uint32 startTime = SDL_GetTicks();

	static Uint32 lastFrame = startTime;

	Uint32 elapsedMS = SDL_GetTicks() - startTime; // Time since start of loop
	if (elapsedMS) { // Skip this the first frame
		double elapsedSeconds = elapsedMS / 1000.0; // Convert to seconds
		double fps = numFrames / elapsedSeconds; // FPS is Frames / Seconds
		//SDL_Delay(1000.0 / 60.0); // Use floating point division, not integer
	}

	if (SDL_GetTicks() < lastFrame + 1000.0f/60.0f )
	{
		return;
	}
	else
	{
		++numFrames;
		lastFrame = SDL_GetTicks();
		char fps[10];
		sprintf(fps,"%lf", numFrames / (elapsedMS / 1000.0));

		pspDebugScreenPrintf(fps);
	}

	// SDL_SetRenderDrawColor(Gfx.GetRenderer(), 255, 255, 255, 255);

	switch (State)
	{
	case DEBUG:
		for (int i = 0; i < RES_Y; i++)
		{
			for (int j = 0; j < RES_X; j++)
			{
				if (myChip8.GetGfx()[i * RES_X + j] == 0)
				{
					pspDebugScreenPrintf(" ");
				}
				else pspDebugScreenPrintf("#");
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

int done = 0;

// void renderer_loop(SDL_Renderer* renderer)
// {
// 	int i;

// 	for (i = 0; !done; ++i)
// 	{
// 		// Set render color to red ( background will be rendered in this color )
// 		SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );

// 		// Clear winow
// 		SDL_RenderClear( renderer );

// 		// Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
// 		SDL_Rect r;
// 		r.x = 50 + ((i % 3) * 10);
// 		r.y = 50;
// 		r.w = 50;
// 		r.h = 50;

// 		// Set render color to blue ( rect will be rendered in this color )
// 		SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );

// 		for (int i = 0; i < RES_Y; i++)
// 		{
// 			for (int j = 0; j < RES_X; j++)
// 			{
// 				if (myChip8.GetGfx()[i * RES_X + j] != 0)
// 				{
// 					Gfx.Draw_sprite(j*SCALE, i*SCALE);
// 				}
// 			}
// 		}

// 		// Render rect
// 		// SDL_RenderFillRect( renderer, &r );

// 		// Render the rect to the screen
// 		SDL_RenderPresent(renderer);

// 		// Wait for 1 sec
// 		if (!done)
// 		{
// 			SDL_Delay( 1000 );
// 		}
// 	}
// }


int main(int argc, char **argv)
{
	// Set up render system and register input callbacks
	if(State != DEBUG) Gfx.SDL_Gfx_Init(); // Opengl/SDL part
	// input.SDL_Event_Init();	//SDL part

	setupCallbacks();
    pspDebugScreenInit();


	// SDL_Window* window = SDL_CreateWindow
    // (
    //     "BRIAN_TEST WINDOW", SDL_WINDOWPOS_UNDEFINED,
    //     SDL_WINDOWPOS_UNDEFINED,
    //     640,
    //     480,
    //     SDL_WINDOW_SHOWN
    // );

    // if (window == NULL)
    // {
    //     SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	// 	return 1;
	// }

    // // Setup renderer
    // SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
    // if (renderer != NULL)
    // {
	// 	renderer_loop(renderer);
	// 	SDL_DestroyRenderer(renderer);
	// 	renderer = NULL;
	// }

    // SDL_DestroyWindow(window);
    // window = NULL;

    // SDL_Quit();

    // pspDebugScreenSetXY(0, 0);
	
	sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	// Initialize the Chip8 system and load the game into the memory  
	myChip8.initialize();
	
	/*myChip8.Compile("TEST");
	myChip8.Dissasemble("TEST");
	*/
	
	myChip8.loadGame("PONG");

	
	// int FPS = 50; //Framerate


	// Emulation loop
	for (;;)// while(1) equivalent
	{
		// Emulate one cycle
		sceCtrlReadBufferPositive(&padData, 1);
        // if(padData.Buttons & PSP_CTRL_CROSS){
			myChip8.emulateCycle();
		// }
		// If the draw flag is set, update the screen
		if (myChip8.drawFlag)
			drawGraphics();//Opengl/SDl part

		// Store key press state (Press and Release)
		myChip8.setKeys();

		// update timers
		/*myChip8.Update_Timer();
		myChip8.Update_Sound();*/

		// input.Event_handler(myChip8,Gfx);
		input.PSP_Controls(latch, padData, myChip8, Gfx);

		if (Gfx.Exit_App() == true)
			break;
		// execute at 60 hz
		// sceKernelDelayThread(1000.0f/960.0f);
		
		

		// if (100>(SDL_GetTicks() - startTime))
		// {
		// 	SDL_Delay((100/FPS) - (SDL_GetTicks() - startTime)); //SDL_Delay pauses the execution.
		// }

		if(State == DEBUG)
			pspDebugScreenClear();

		/*cout << "Register Map" << endl;
		for (int i = 0; i < 16; i++)
		{
			printf("0x%X\n",myChip8.GetV()[i]);
		}
		system("cls");*/
	}

	return 0;
}