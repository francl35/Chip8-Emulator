#pragma once
#include "chip8.h"
#include "Graphics.h"
#include <pspctrl.h>

#include <SDL2/SDL.h>

class Input
{
	SDL_Event m_event;
public:
	Input();
	~Input();
	void SDL_Event_Init();
	void Event_handler(chip8 &chip, Graphics &gfx);
	void PSP_Controls(SceCtrlLatch latch, SceCtrlData padData, chip8 &chip, Graphics &gfx);
};

