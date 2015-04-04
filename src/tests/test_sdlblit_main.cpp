#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>
using std::cerr;
using std::cout;
using std::vector;

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

//======================================================================

class Color
{
public:
	static uint32_t const RedShift = 16;
	static uint32_t const GreenShift = 8;
	static uint32_t const BlueShift = 0;

	static uint32_t const RedMask = 0xFFU << RedShift;
	static uint32_t const GreenMask = 0xFFU << GreenShift;
	static uint32_t const BlueMask = 0xFFU << BlueShift;
	static uint32_t const AlphaMask = 0;
	
public:
	Color () {}
	inline Color (uint8_t r, uint8_t g, uint8_t b);
	inline operator uint32_t () const;

	inline uint8_t r () const;
	inline uint8_t g () const;
	inline uint8_t b () const;

private:
	uint32_t m_color;
};

static_assert (sizeof(Color) == 4, "Color should be 4 bytes.");

//======================================================================

struct Sprite
{
	SDL_Texture * tex;
	int w, h;

	Sprite (int w_, int h_, SDL_Texture * tex_): tex (tex_), w (w_), h (h_) {assert(nullptr != tex);}
	~Sprite () {SDL_DestroyTexture(tex);}

	size_t burden () const {return sizeof(Color) * w * h;}
	size_t pixelCount () const {return w * h;}
};

//======================================================================

struct Object
{
	Sprite * sprite;
	int x, y;

	Object (int pos_x, int pos_y, Sprite * sprite_) : sprite (sprite_), x (pos_x), y (pos_y) {}

	size_t burdenRead () const {return sprite->burden();}
	size_t burdenWrite () const {return sprite->burden();}
	size_t pixelCount () const {return sprite->pixelCount();}
};

//======================================================================

SDL_Texture * CreateSolidColorTexture (SDL_Renderer * renderer, int w, int h, Color c);
vector<Sprite> SetupSprites (SDL_Renderer * renderer, int n, int w_min, int h_min, int w_max, int h_max);
vector<Object> SetupObjects (int n, int screen_w, int screen_h, vector<Sprite> & sprites);

//======================================================================

template <typename T>
T RandInt (T lo, T hi)	// both inclusive
{
	return (rand() * rand()) % (hi - lo + 1) + lo;
}

//======================================================================

int main ()
{
	SDL_SetMainReady ();

	srand(unsigned(time(nullptr)));

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

	/* Setup resources and "state" */
	int screen_width = 0, screen_height = 0;
	SDL_GetWindowSize (window, &screen_width, &screen_height);

	auto texture = CreateSolidColorTexture (renderer, 2, 2, Color(0, 255, 255));
	auto sprites = SetupSprites (renderer, 1000, 20, 20, 100, 100);
	auto objects = SetupObjects (20000, screen_width, screen_height, sprites);

	SDL_SetRenderDrawColor (renderer, 5, 5, 20, 255);

	size_t total_read_burden = 0;
	size_t total_write_burden = 0;
	size_t total_pixels = 0;
	for (auto const & obj : objects)
	{
		total_read_burden += obj.burdenRead();
		total_write_burden += obj.burdenWrite();
		total_pixels += obj.pixelCount();
	}
	cout << " Total read burden: " << total_read_burden << " bytes/frame\n";
	cout << "Total write burden: " << total_write_burden << " bytes/frame\n";
	cout << "      Total pixels: " << total_pixels << " per frame\n";

	/* Into the event loop we go... */
	uint32_t last_time = SDL_GetTicks ();
	uint32_t last_frames = 0;

	uint32_t frames = 0;
	SDL_Event ev;
	for (;;)
	{
		int poll_res = SDL_PollEvent(&ev);

		if (0 != poll_res)	// We actually do have an event to process
		{
			//cout << "[INFO] " << ev.common.timestamp << ", " << ev.type << "\n";
			
			if (SDL_QUIT == ev.type)
				break;
		}
		else
		{
			// THE RENDER!
			SDL_RenderClear (renderer);

			SDL_Rect src, dst;
			for (auto const & obj : objects)
			{
				src.x = 0;
				src.y = 0;
				src.w = obj.sprite->w;
				src.h = obj.sprite->h;

				dst.x = obj.x;
				dst.y = obj.y;
				dst.w = src.w;
				dst.h = src.h;

				SDL_RenderCopy (renderer, obj.sprite->tex, &src, &dst);
			}

			SDL_Rect from {0, 0, 2, 2};
			SDL_Rect to {10, 10, 20, 20};
			SDL_RenderCopy (renderer, texture, &from, &to);

			SDL_RenderPresent (renderer);
			frames += 1;
		}

		uint32_t curr_time = SDL_GetTicks ();
		if (curr_time - last_time >= 1000)
		{
			double frame_time = double(curr_time - last_time) / (frames - last_frames);
			last_frames = frames;
			last_time = curr_time;
			cout << "Average frame time: " << frame_time << "ms\n";
		}
	}
	
	SDL_DestroyTexture (texture);
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	SDL_Quit ();
	return 0;
}

//======================================================================

inline Color::Color (uint8_t r, uint8_t g, uint8_t b)
	: m_color ((r << RedShift) | (g << GreenShift) | (b << BlueShift))
{
}

//----------------------------------------------------------------------

inline Color::operator uint32_t () const
{
	return m_color;
}

//----------------------------------------------------------------------

inline uint8_t Color::r () const
{
	return (m_color >> RedShift) && 0xFF;
}

//----------------------------------------------------------------------

inline uint8_t Color::g () const
{
	return (m_color >> GreenShift) && 0xFF;
}

//----------------------------------------------------------------------

inline uint8_t Color::b () const
{
	return (m_color >> BlueShift) && 0xFF;
}

//----------------------------------------------------------------------
//======================================================================

SDL_Texture * CreateSolidColorTexture (SDL_Renderer * renderer, int w, int h, Color c)
{
	Color * pixels = new Color [h * w];
	std::fill (pixels, pixels + h * w, c);

	SDL_Surface * surface = SDL_CreateRGBSurfaceFrom (
		pixels, w, h, 8 * sizeof(Color), w * sizeof(*pixels),
		Color::RedMask, Color::GreenMask, Color::BlueMask, Color::AlphaMask
	);
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

vector<Sprite> SetupSprites (SDL_Renderer * renderer, int n, int w_min, int h_min, int w_max, int h_max)
{
	vector<Sprite> ret;
	ret.reserve (n);

	for (int i = 0; i < n; ++i)
	{
		int w = RandInt(w_min, w_max);
		int h = RandInt(h_min, h_max);
		ret.emplace_back (
			w, h,
			CreateSolidColorTexture (
				renderer, w, h,
				Color(RandInt<uint8_t>(0, 255), RandInt<uint8_t>(0, 255), RandInt<uint8_t>(0, 255))
			)
		);
		assert (nullptr != ret.back().tex);
	}

	return ret;
}

//----------------------------------------------------------------------

vector<Object> SetupObjects (int n, int screen_w, int screen_h, vector<Sprite> & sprites)
{
	vector<Object> ret;
	ret.reserve (n);

	for (int i = 0; i < n; ++i)
	{
		int s = RandInt(0, int(sprites.size() - 1));
		ret.emplace_back (
			RandInt(0, screen_w - sprites[s].w), RandInt(0, screen_h - sprites[s].h),
			&(sprites[s])
		);
	}

	return ret;
}

//======================================================================
