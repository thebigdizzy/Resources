#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#endif

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"
#endif

// needed includes
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

#include <stdlib.h>

class Enemy{
	public:
	// bool for the state of the enemy
	bool active;

	// enemy texture
	SDL_Texture *texture;

	// enemy rectangle fotr position (x, y) and size (w,h)
	SDL_Rect posRect;

	// movement direction values
	float xDir, yDir;

	// enemy speed
	float speed;

	// enemy float positions to prevent precision loss
	float pos_X, pos_Y;

	// enemy angle
	float angle;

	// point for the center of the object. used for rotation
	SDL_Point center;

	// enemy creation method. Requires the renderer, a path to the needed image
	Enemy(SDL_Renderer *renderer, string filePath);

	// enemy update = requires deltaTime be passed
	void Update(float deltaTime);

	// enemy draw - requires renderer be passed
	void Draw(SDL_Renderer *renderer);

	// enemy reset
	void Reset();

	// enemy destruction Method
	~Enemy();
};
