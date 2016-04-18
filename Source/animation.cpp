#include "animation.h"

// Animate creation method
Animate::Animate(SDL_Renderer *renderer, string filePath, float x, float y, int id)
{
	// set the explosion initial state 
	active = false;

	idNum = id;

	string AnimatePath;

	// get the directory path for the explosion graphic
	AnimatePath = filePath + "Copper.png";

	// init frame counter
	frameCounter = 0.0f;

	// load the texture into a surface
	SDL_Surface *surface = IMG_Load(AnimatePath.c_str());

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

	// setting the individualimage size
	posRect.w = 156/3;
	posRect.h = 190/3;

	// set the x and y position of the explosion rectangle
	posRect.x = x;
	posRect.y = y;

	// frame width and height of each animation image

	frameWidth = 156;
	frameHeight = 190;
	drawRect.x = 156;
	drawRect.y = 0;

	// original draw rectangle
	// used to draw one frame of the animation strip


	drawRect.w = frameWidth;
	drawRect.h = frameHeight;
}

// explosion update method
void Animate::Update(float deltaTime) // float delta
{
	// check to see if the bullet is active
	if (active)
	{
		// add to frame counter 
		frameCounter += deltaTime;

		// check to see if enough time has passed
		if (frameCounter >= .05f)
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
				//active = false;

				// move off screen until needed again
				//posRect.x = -1000;
				//posRect.y = -1000;
			}
		}
	}
}

// reset the explosion method
void Animate::Reset()
{
	// deactivate the explosion
	active = false;
}

// explosion draw method
void Animate::Draw(SDL_Renderer *renderer, float angle)
{
	SDL_RenderCopyEx(renderer, texture, &drawRect, &posRect, angle, NULL, SDL_FLIP_NONE);
}

// explosion destruction
Animate::~Animate()
{
	// SDL_DestroyTexture(texture);
}
