#include "Graphics.h"  // OpenGL and SDL graphics and input
#include "Input.h"
#include "chip8.h" // Your cpu core implementation
#ifdef _WIN32
	#include <windows.h>                // for Windows APIs
#else
	#include <unistd.h>
#endif
#include <chrono>
#include <numeric>


chip8 myChip8;
Graphics Gfx;
Input input;
gfx_state State = OPENGL;

void drawGraphics()
{
	static int numFrames = 0;
	static Uint32 startTime = SDL_GetTicks();

	static Uint32 lastFrame = startTime;

	// Uint32 elapsedMS = SDL_GetTicks() - startTime; // Time since start of loop
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
	case OPENGL:
	{
		// SDL_SetRenderDrawColor(Gfx.GetRenderer(), 255, 255, 255, 255);
		// for (int i = 0; i < RES_Y; i++)
		// {
		// 	for (int j = 0; j < RES_X; j++)
		// 	{
		// 		if (myChip8.GetGfx()[i * RES_X + j] != 0)
		// 		{
		// 			Gfx.Draw_sprite(j*SCALE, i*SCALE);
		// 		}
		// 	}
		// }
		break;
	}
	default:
		break;
	}
}

int main(int argc, char **argv)
{
	// Set up render system and register input callbacks
	if(State == DEBUG) Gfx.SDL_Gfx_Init(); // SDL part
	if(State == OPENGL) Gfx.GL_Gfx_Init(); // Opengl
	input.SDL_Event_Init();	//SDL part
	myChip8.drawFlag = false;


	// Initialize the Chip8 system and load the game into the memory  
	myChip8.initialize();
	
	/*myChip8.Compile("TEST");
	myChip8.Dissasemble("TEST");
	*/
	
	myChip8.loadGame((char*)"c8games/PONG");

	// int FPS = 50; //Framerate

	std::vector<double> emulate_times;
	
	


	


	// ImGui_ImplSDL2_NewFrame(Gfx.GetWindow());
    // ImGui::NewFrame();

	// ImGui::Begin("chip8");                          // Create a window called "Hello, world!" and append into it.
    // ImGui::End();
    

	// Emulation loop
	for (;;)// while(1) equivalent
	{

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		ImGui::NewFrame();

		ImGui::Begin("My First Tool", NULL, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
    	if (ImGui::BeginMenu("File"))
    	{
    	    if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
    	    if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
    	    ImGui::EndMenu();
    	}
    	ImGui::EndMenuBar();
		}
		ImGui::End();

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    	ImGui::SameLine();
    	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    	ImGui::End();

		ImGui::Render();

    	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    	glClear(GL_COLOR_BUFFER_BIT);
    	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    	SDL_GL_SwapWindow(Gfx.GetWindow());

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
			//system("pause");
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
		//sleep(1000.0f/960.0f);
		// SDL_Delay(1000.0f / 960.0f);
		
		
		

		/*if (100>(SDL_GetTicks() - startTime))
		{
			SDL_Delay((100/FPS) - (SDL_GetTicks() - startTime)); //SDL_Delay pauses the execution.
		}*/

		if(State == DEBUG)
		{
			#ifdef _WIN32
				system("cls");
			#else 
				system("clear");
			#endif
		}


		/*cout << "Register Map" << endl;
		for (int i = 0; i < 16; i++)
		{
			printf("0x%X\n",myChip8.GetV()[i]);
		}
		system("cls");*/
	}

	// Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // SDL_GL_DeleteContext(gl_context);
    // SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}
