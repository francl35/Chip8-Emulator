#pragma once
#include <SDL.h>
#include "chip8.h"
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
//#include <gl/GL.h>
//#include <gl/GLU.h>
//#include <SDL_opengl.h>

#define SCALE 5
class Graphics
{
	SDL_Window* m_window = NULL;
	SDL_GLContext gl_context = NULL;

	SDL_Renderer* m_background_renderer = NULL;
	SDL_Renderer* m_sprite_renderer = NULL;
	bool exit_value;
	int counter;
	
public:
	Graphics();
	~Graphics();
	int SDL_Gfx_Init();
	int GL_Gfx_Init();
	void Draw_sprite(int pos_x,int pos_y);
	void SDL_QuitApp();
	bool Exit_App();
	SDL_Renderer* GetRenderer() { return m_background_renderer; };
	void GetFPS();
};
