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


class Key {
public:
	// is the jewel active
	bool active;

	SDL_Texture *key;

	SDL_Rect keyRect, sRect;

	float posJ_X, posJ_Y;

	Key(SDL_Renderer *renderer, string filePath, float x, float y);

	void Draw(SDL_Renderer *renderer);

	void Update(float deltaTime);

	void Reset();

	void TankMoveX(float tankSpeed, float deltaTime);

	void TankMoveY(float tankSpeed, float deltaTime);
};
