#include "rockHit.h"

// rockHit creation method
RockHit::RockHit(SDL_Renderer *renderer, string filePath, float x, float y, int id)
{
	// set the explosion initial state 
	active = false;

	// get the directory path for the explosion graphic
	string rockHitPath = filePath + "blueExplosion.png";

	// init frame counter
	frameCounter = 0.0f;

	// load the texture into a surface
	SDL_Surface *surface = IMG_Load(rockHitPath.c_str());

	// create the texture from the passed renderer and created surface
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface for later use
	SDL_FreeSurface(surface);

	// set the width and height of the bullet's rectangle
	// by querying the texture itself
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	// getting the width and heioght of the entire image
	textureWidth = w;
	textureHeight = h;

	// setting the individual image size
	posRect.w = 100;
	posRect.h = 100;

	// set the x and y position of the explosion rectangle
	posRect.x = x;
	posRect.y = y;

	// frame width and height of each animation image
	frameWidth = 192;
	frameHeight = 192;

	// original draw rectangle
	// used to draw one frame of the animation strip
	drawRect.x = 192;
	drawRect.y = 0;
	drawRect.w = frameWidth;
	drawRect.h = frameHeight;
}

// explosion update method
void RockHit::Update(float deltaTime) // float delta
{
	// check to see if the bullet is active
	if (active)
	{
		// add to frame counter 
		frameCounter += deltaTime;

		// check to see if enough time has passed
		if (frameCounter >= .1f)
		{
			// reset frame counter
			frameCounter = 0.0f;

			// add to the drawRect the width of an individual fram
			// to move the drawing area over one
			drawRect.x += frameWidth;
			// see if we have gone past the edge of the image - width
			if (drawRect.x >= textureWidth)
			{
				// if so go back to the first = 0
				drawRect.x = 0;

				// set active set to false
				active = false;

				// move off screen until needed again
				posRect.x = -1000;
				posRect.y = -1000;
			}
		}
	}
}

// reset the explosion method
void RockHit::Reset()
{
	// deactivate the explosion
	active = false;
}

// explosion draw method
void RockHit::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, &drawRect, &posRect);
}

// explosion destruction
RockHit::~RockHit()
{
	// SDL_DestroyTexture(texture);
}
