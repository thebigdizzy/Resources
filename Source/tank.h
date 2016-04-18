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

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "tankBullet.h"

using namespace std;

class Tank{
public:
	// textures for health bar
	SDL_Texture *back, *mid, *front;

	// rectangles for the health bar images
	SDL_Rect backR, midR, frontR;

	// number of rocks bibble has
	int rocks;

	// player health
	float playerHealth, maxHealth;

	// tank hit by eTank
	void eTankHit();

	// tank hit by turret bullet
	void eBulletHit();

	bool active;

	// variable to hold the list of bullets
	vector<Rocks> bulletList;

	string playerPath;

	int playerNum;

	SDL_Texture *texture;

	SDL_Rect posRect;

	float xDir, xDirOld;
	float yDir, yDirOld;

	float speed;
	float pos_X, pos_Y;

	// add x and y values
	Sint16 Xvalue, Yvalue;

	float x,y,tankAngle;

	SDL_Point center;

	// keep the last angle so the player can fire in the correct direction
	float oldAngle;

	// audio sound effect - Chunk
	//Mix_Chunk *fire;

	Tank(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y);

	void OnControllerAxis(Sint16 X, Sint16 Y);

	void OnControllerButton(const SDL_ControllerButtonEvent event);

	void Update (float deltaTime);

	void Draw(SDL_Renderer *renderer);

	void Reset();

private:
	void CreateBullet();
};
