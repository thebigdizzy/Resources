#include <string>
#include <iostream>
using namespace std;

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

class TankBullet {
public:
	// bool for the state of the bullet
	bool active;

	// bullet texture
	SDL_Texture *texture;

	// bulllet rectangle for position (X, Y) and size (W, H)
	SDL_Rect posRect;

	// movement direction values
	// float xDir, yDir;

	// bullet speed
	float speed;

	// tank angle passed in
	float tankAngle;

	// bullet float positions to prevent precision loss
	float pos_X, pos_Y;

	// bullet creation method. requires the renderer, and path to the needed image, an x position, and a y position
	TankBullet(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY);

	// bullet update - requires deltaTime be passed
	void Update(float deltaTime);

	// bullet draw - requires renderer be passed
	void Draw(SDL_Renderer *renderer);

	// bullet reset
	void Reset();
};
