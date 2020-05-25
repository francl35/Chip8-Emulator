#include "Input.h"

Input::Input()
{
}


Input::~Input()
{
}

void Input::SDL_Event_Init()
{

}

void Input::Event_handler(chip8 &chip, Graphics &gfx)
{
		/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
		/* more events on the event queue, our while loop will exit when */
		/* that occurs.                                                  */
		while (SDL_PollEvent(&m_event)) {
			/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
			switch (m_event.type) {
			case SDL_KEYDOWN:
				/* Check the SDLKey values and move change the coords */
				switch (m_event.key.keysym.sym) {
				case SDLK_1:
					chip.setKey(0x0, 1);
					break;
				case SDLK_2:
					chip.setKey(0x1, 1);
					break;
				case SDLK_3:
					chip.setKey(0x2, 1);
					break;
				case SDLK_4:
					chip.setKey(0x3, 1);
					break;
				case SDLK_a:
					chip.setKey(0x4, 1);
					break;
				case SDLK_z:
					chip.setKey(0x5, 1);
					break;
				case SDLK_e:
					chip.setKey(0x6, 1);
					break;
				case SDLK_r:
					chip.setKey(0x7, 1);
					break;
				case SDLK_q:
					chip.setKey(0x8, 1);
					break;
				case SDLK_s:
					chip.setKey(0x9, 1);
					break;
				case SDLK_d:
					chip.setKey(0xA, 1);
					break;
				case SDLK_f:
					chip.setKey(0xB, 1);
					break;
				case SDLK_w:
					chip.setKey(0xC, 1);
					break;
				case SDLK_x:
					chip.setKey(0xD, 1);
					break;
				case SDLK_c:
					chip.setKey(0xE, 1);
					break;
				case SDLK_v:
					chip.setKey(0xF, 1);
					break;
				case SDLK_ESCAPE:
					gfx.SDL_QuitApp();
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				/* Check the SDLKey values and move change the coords */
				switch (m_event.key.keysym.sym) {
				case SDLK_1:
					chip.setKey(0x0, 0);
					break;
				case SDLK_2:
					chip.setKey(0x1, 0);
					break;
				case SDLK_3:
					chip.setKey(0x2, 0);
					break;
				case SDLK_4:
					chip.setKey(0x3, 0);
					break;
				case SDLK_a:
					chip.setKey(0x4, 0);
					break;
				case SDLK_z:
					chip.setKey(0x5, 0);
					break;
				case SDLK_e:
					chip.setKey(0x6, 0);
					break;
				case SDLK_r:
					chip.setKey(0x7, 0);
					break;
				case SDLK_q:
					chip.setKey(0x8, 0);
					break;
				case SDLK_s:
					chip.setKey(0x9, 0);
					break;
				case SDLK_d:
					chip.setKey(0xA, 0);
					break;
				case SDLK_f:
					chip.setKey(0xB, 0);
					break;
				case SDLK_w:
					chip.setKey(0xC, 0);
					break;
				case SDLK_x:
					chip.setKey(0xD, 0);
					break;
				case SDLK_c:
					chip.setKey(0xE, 0);
					break;
				case SDLK_v:
					chip.setKey(0xF, 0);
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
}
