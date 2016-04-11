#include "tankBullet.h"

// bullet creation method
TankBullet::TankBullet(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY)
{
	// set the bullet initial state
	active = false;

	// set bullet speed
	speed = 800;

	// create the texture from the passed renderer and created surface
	texture = IMG_LoadTexture(renderer, filePath.c_str());

	// set the width and height of the bullet's rect
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	// set the x and y pos of the bullet rect
	// finish alighning to the player center using the texture width
	posRect.x = x - (posRect.w/2);
	posRect.y = y;

	// set the float positon values of the texture for precision loss
	pos_X = x;
	pos_Y = y;

	// initialize the direction vars for the bullet
	// no x movement, no y movement
	//xDir = dirX;
	//yDir = dirY;
}

// reset the bullet method
void TankBullet::Reset()
{
	// reset the x position off the screen
	posRect.x = -1000;

	// update the pos+X for precision
	pos_X = posRect.x;

	// deactivate the bullet
	active = false;
}

// bullet draw method
void TankBullet::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

// bullet update method
void TankBullet::Update(float deltaTime)
{
		// adjust position floats based on speed, direction (-1 for up), and deltaTime
		//pos_X += (speed * xDir) * deltaTime;
		//pos_Y += (speed * yDir) * deltaTime;

	float radians = (tankAngle * 3.14)/180;

	float move_x = speed * cos(radians);
	float move_y = speed * sin(radians);

	pos_X += (move_x) * deltaTime;
	pos_Y += (move_y) * deltaTime;

		// update bullet position with code to account for precision loss
		posRect.x = (int)(pos_X + .5f);
		posRect.y = (int)(pos_Y + .5f);

		// check to see if the bullet is off the top of the screen
		// and deactivate and move off screen
		if((posRect.y < (0 - posRect.h)) || (posRect.y > 768) || (posRect.x < (0 - posRect.w)) || (posRect.x > 1024))
		{
			Reset();
		}
}
