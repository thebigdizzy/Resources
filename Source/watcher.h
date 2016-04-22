#include <time.h>
#include <string>
#include <iostream>

using namespace std;
#include <vector>

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

#include "watcherLight.h"

class Watcher{
public:
	// is the Watcher active
	bool active;

	// variable to hold the list of bullets
	vector<WatcherLight> lightList;

	// string to hold the Watcher's texture
	string WatcherPath;

	// Watcher's texture to hold base abd barrel images
	SDL_Texture *tBase;
	SDL_Texture *tBarrel;

	// Watcher's SDL_Rect for the x, y, w, and h of base texture
	SDL_Rect baseRect, sRect;

	// barrel's SDL_Rect for the x, y, w, and h of barrel's texture
	SDL_Rect barrelRect;

	// Watcher's SDL_Rect for the angle between the Watcher's position and the tank's position
	float x, y, WatcherAngle;

	// point that will be used to rotate the Watcher - relative to the Watcher barrel's Rect
	SDL_Point center;

	// floats for the fireTime  and fireRate
	float fireTime = 0.0f;
	float fireRate = 1500.0f;

	// bool to tell when the watcher has shot out a light
	bool shot;

	// audio sound effect - CHUNK
	Mix_Chunk *fire;

	// Watcher's creation method using passed in values for renderer, player number, path to the texture
	// starting pos x, starting pos y
	Watcher(SDL_Renderer *renderer, string filrPath, string audioPath, float x, float y);

	// update the Watcher using the passed in deltaTime
	void Update(float deltaTime, SDL_Rect tankRect);

	// draw the Watcher main's passed in renderer
	void Draw(SDL_Renderer *renderer);

	// reset the Watcher
	void Reset();

	// create a bullet
	void CreateBullet(SDL_Rect target);

	void TankMoveX(float tankSpeed, float deltaTime);

	void TankMoveY(float tankSpeed, float deltaTime);

	// float for the Watcher's position to avoid precision loss
	float posB_X, posB_Y, posT_X, posT_Y, sPosX, sPosY;

};
