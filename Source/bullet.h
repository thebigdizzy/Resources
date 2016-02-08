#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#endif

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#endif

#include <string>
#include <iostream>
using namespace std;

class Bullet {
public:

	// boolean for the state of the bullet
	bool active;

	// bullet texture
	SDL_Texture *texture;

	// bullet rectangle for position (x,y) and size (w,h)
	SDL_Rect posRect;

	// movement direction values
	float xDir, yDir;

	// Bullet speed
	float speed;

	// bullet float positions to prevent precision loss 
	float pos_X, pos_Y;

	// bullet creation method. requires the renderer, a path to the needed image, an x position, a y position
	Bullet(SDL_Renderer *renderer, string filePath, float x, float y);

	// bullet update - requires deltaTime be passed
	void Update(float deltaTime);

	// bullet draw = requires renderer be passed
	void Draw(SDL_Renderer *renderer);

	// bullet destruction method
	~Bullet();
};