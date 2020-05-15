#pragma once
#include "chip8.h"
#include "Graphics.h"
#include <SDL.h>

class Input
{
	SDL_Event m_event;
public:
	Input();
	~Input();
	void SDL_Event_Init();
	void Event_handler(chip8 &chip, Graphics &gfx);
};

