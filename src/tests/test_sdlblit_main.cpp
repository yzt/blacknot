#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
using std::cerr;
using std::cout;

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

//======================================================================

SDL_Texture * CreateSolidColorTexture (SDL_Renderer * renderer, int w, int h, uint32_t color);

//======================================================================

int main ()
{
	SDL_SetMainReady ();

	/* Init SDL... */
	int init_res = SDL_Init (SDL_INIT_VIDEO);
	if (0 != init_res)
	{
		cerr << "[ERROR] SDL init failed: " << SDL_GetError() << "\n";
		return 1;
	}

	/* Create window... */
	SDL_Window * window = SDL_CreateWindow (
		"Blacknot SDL Blit Test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 400,
		0
	);
	if (nullptr == window)
	{
		cerr << "[ERROR] Window creation failed: " << SDL_GetError() << "\n";
		SDL_Quit ();
		return 2;
	}

	/* Create renderer... */
	SDL_Renderer * renderer = SDL_CreateRenderer (window, 0, SDL_RENDERER_ACCELERATED);
	if (nullptr == renderer)
	{
		cerr << "[ERROR] Renderer creation failed: " << SDL_GetError() << "\n";
		SDL_DestroyWindow (window);
		SDL_Quit ();
		return 3;
	}

	SDL_Texture * texture = CreateSolidColorTexture (renderer, 2, 2, 0x0000FFFF);

	SDL_SetRenderDrawColor (renderer, 5, 5, 20, 255);

	/* Into the event loop we go... */
	SDL_Event ev;
	for (;;)
	{
		int poll_res = SDL_PollEvent(&ev);

		if (0 != poll_res)	// We actually do have an event to process
		{
			cout << "[INFO] " << ev.common.timestamp << ", " << ev.type << "\n";
			
			if (SDL_QUIT == ev.type)
				break;
		}
		else
		{
			SDL_RenderClear (renderer);

			// Do the render here...
			SDL_Rect from {0, 0, 2, 2};
			SDL_Rect to {10, 10, 20, 20};

			SDL_RenderCopy (renderer, texture, &from, &to);

			SDL_RenderPresent (renderer);
		}
	}
	
	SDL_DestroyTexture (texture);
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	SDL_Quit ();
	return 0;
}

//======================================================================

SDL_Texture * CreateSolidColorTexture (SDL_Renderer * renderer, int w, int h, uint32_t color)
{
	uint32_t * pixels = new uint32_t [h * w];
	std::fill (pixels, pixels + h * w, color);

	SDL_Surface * surface = SDL_CreateRGBSurfaceFrom (pixels, w, h, 32, w * sizeof(*pixels), 0x00FF0000, 0x0000FF00, 0x000000FF, 0);
	if (nullptr == surface)
	{
		delete[] pixels;
		return nullptr;
	}

	SDL_Texture * ret = SDL_CreateTextureFromSurface (renderer, surface);

	SDL_FreeSurface (surface);
	delete[] pixels;
	return ret;
}

//----------------------------------------------------------------------
//======================================================================
