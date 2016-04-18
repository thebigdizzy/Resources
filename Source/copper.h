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

#include <string>
#include <iostream>

using namespace std;
#include <vector>
#include "animation.h"

class Copper {
public:
	// is the Copper active = still alive?
	bool active;

	// directional control
	bool rt, lt, up, dn, turn;

	// initial position
	float sPosX, sPosY;
	float posX, posY;

	string eCopperPath;

	SDL_Texture *eCopper;

	SDL_Rect eCopperRect;

	float x, y, CopperAngle;

	vector<Animate> cAnim;

	Mix_Chunk *explode;

	float speed;

	int health;

	SDL_Point center;

	Copper(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y);

	void Update(float deltaTime, SDL_Rect CopperRect);

	void Draw(SDL_Renderer *renderer);

	void Reset();

	void MakeCopper(int x, int y);

	void RemoveHealth();

	void eCopperMoveX(float CopperSpeed, float deltaTime);

	void eCopperMoveY(float CopperSpeed, float deltaTime);

	float posT_X, posT_Y;
};