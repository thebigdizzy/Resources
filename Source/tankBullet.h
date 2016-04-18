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

class Rocks {
public:
	// bool for the state of the bullet
	bool active;

	bool stop;

	// bullet texture
	SDL_Texture *texture;

	// bulllet rectangle for position (X, Y) and size (W, H)
	SDL_Rect posRect;

	// starting position of bullet (used for distance)
	int sX, sY;

	// distance
	float distance;

	// float for the turret's position to avoid precision loss
	float posB_X, posB_Y, posT_X, posT_Y;

	// movement direction values
	// float xDir, yDir;

	// bullet speed
	float speed;

	// tank angle passed in
	float tankAngle;

	// bullet float positions to prevent precision loss
	float pos_X, pos_Y;

	// bullet creation method. requires the renderer, and path to the needed image, an x position, and a y position
	Rocks(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY);

	void TankMoveX(float tankSpeed, float deltaTime);

	void TankMoveY(float tankSpeed, float deltaTime);

	// bullet update - requires deltaTime be passed
	void Update(float deltaTime);

	// bullet draw - requires renderer be passed
	void Draw(SDL_Renderer *renderer);

	// does the distance formula
	float Distance(float x1, float y1, float y2, float x2);

	// bullet reset
	void Reset();
};
