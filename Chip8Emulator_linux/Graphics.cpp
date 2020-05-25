#include "Graphics.h"

Graphics::Graphics()
{
	exit_value = false;
	counter = 0;
}

Graphics::~Graphics()
{
}

int Graphics::SDL_Gfx_Init()
{
	//Initialization
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stdout, "SDL Initialization error (%s)\n", SDL_GetError());
		return -1;
	}

	{
		//Window creation
		m_window = SDL_CreateWindow("CHIP 8 Emulator", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			RES_X*SCALE,
			RES_Y*SCALE,
			SDL_WINDOW_SHOWN);

		if (m_window)
		{
			// We must call SDL_CreateRenderer in order for draw calls to affect this window.
			m_background_renderer = SDL_CreateRenderer(m_window, -1, 0);

			// Select the color for drawing. It is set to red here.
			SDL_SetRenderDrawColor(m_background_renderer, 0, 0, 0, 255);
			SDL_RenderClear(m_background_renderer);
			// Clear the entire screen to our selected color.
			SDL_RenderClear(m_background_renderer);
			SDL_SetRenderDrawColor(m_background_renderer, 255, 255,255, 255);
			

			// Up until now everything was drawn behind the scenes.
			// This will show the new, red contents of the window.
			//SDL_RenderPresent(m_background_renderer);
			//SDL_Delay(0);

			
		}
		else
		{
			fprintf(stderr, "Windows creation error: %s\n", SDL_GetError());
		}
	}

	return 0;
}

int Graphics::GL_Gfx_Init()
{
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* m_window = SDL_CreateWindow("Chip 8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 272, window_flags);
	SDL_GLContext gl_context = SDL_GL_CreateContext(m_window);
    SDL_GL_MakeCurrent(m_window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

	
	// gl_context =  SDL_GL_CreateContext(m_window);

	ImGui::CreateContext();
  

	 // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(m_window, gl_context);
    ImGui_ImplOpenGL2_Init();

	ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window);
   

	return 0;
}

void Graphics::Draw_sprite(int pos_x, int pos_y)
{
	SDL_Rect rectangle;

	rectangle.x = pos_x;
	rectangle.y = pos_y;
	rectangle.w = 5;
	rectangle.h = 5;
	//SDL_RenderClear(m_background_renderer);
	/*if (counter % 1000 == 0)
	{
		SDL_SetRenderDrawColor(m_background_renderer, 0, 0, 0, 255);//black
		SDL_RenderClear(m_background_renderer);
		SDL_SetRenderDrawColor(m_background_renderer, 255, 255, 255, 255);
	}*/


	SDL_RenderFillRect(m_background_renderer, &rectangle);
	//SDL_RenderDrawPoint(m_background_renderer,pos_x,pos_y);

	counter++;

	if (counter > 10000)
		counter = 0;
}

void Graphics::SDL_QuitApp()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	exit_value = true;
}

bool Graphics::Exit_App()
{
	return exit_value;
}

void Graphics::GetFPS()
{
	/*clock_t deltaTime = 0;
	unsigned int frames = 0;
	double  frameRate = 30;
	double  averageFrameTimeMilliseconds = 33.333;

	clock_t beginFrame = clock();
	clock_t endFrame = clock();

	deltaTime += endFrame - beginFrame;
	frames++;

	//if you really want FPS
	if ((deltaTime / (double)CLOCKS_PER_SEC)*1000.0 > 1000.0) { //every second
		frameRate = (double)frames*0.5 + frameRate*0.5; //more stable
		frames = 0;
		deltaTime -= CLOCKS_PER_SEC;
		averageFrameTimeMilliseconds = 1000.0 / (frameRate == 0 ? 0.001 : frameRate);

		std::cout << "CPU time was:" << averageFrameTimeMilliseconds << std::endl;
	}*/
	
		
}
