#include <iostream>
using std::cerr;
using std::cout;

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

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

	/* Into the event loop we go... */
	SDL_Event ev;
	while (true)
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
			// Do the render here...
		}
	}
	
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	SDL_Quit ();
	return 0;
}
