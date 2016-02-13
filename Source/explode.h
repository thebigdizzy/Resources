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

// needed includes
#include <string>
#include <iostream>
using namespace std;

class Explode {
public:
	// bool for the state of the explosion
	bool active;

	// explosion texture
	SDL_Texture *texture;

	// Explosion rectangle for position (x,y) and size (w,h)
	SDL_Rect posRect;

	// explosion rectangle for position (x,y) and size (w,h)
	SDL_Rect drawRect;

	// width and height of eact frame plus width and height of entire image
	int frameWidth, frameHeight, textureWidth, textureHeight;

	// float values to track time until next frame of animation
	float frameCounter;

	// explode creation method. requires the renderer, a path to the needed image, and x position, a y position
	Explode(SDL_Renderer *renderer, string filePath, float x, float y);

	// Explode Update - requires deltaTime to be passed
	void Update(float deltaTime);

	// Explode Draw - requires renderer to be passed
	void Draw(SDL_Renderer *renderer);

	// Explode reset
	void Reset();

	// Explode destruction method
	~Explode();
};
