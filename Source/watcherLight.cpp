#include "watcherLight.h"

// bullet cretion method
WatcherLight::WatcherLight(SDL_Renderer * renderer, string filePath, float x, float y)
{
	// set the bullet initial state
	active = false;

	// set the bullet speed
	speed = 100.0f;

	// create the texture from the passed Renderer and created surface
	texture = IMG_LoadTexture(renderer, filePath.c_str());

	// set the width and height of the bullet's rectangle
	// by querying the texture itself
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	// set the x and y positions of the bullet rectangle
	// finishing aligning to the player center using the texture width
	posRect.x = x;
	posRect.y = y;

	// set the float position values of the texture for precision loss
	pos_X = x;
	pos_Y = y;
}

// reset the bullet method
void WatcherLight::Start(SDL_Rect playerPos, SDL_Rect turretPos)
{
	player.x = playerPos.x;
	player.y = playerPos.y;

	turret.x = turretPos.x;
	turret.y = turretPos.y;

	distance = sqrt((player.x - turret.x) * (player.x - turret.x) + (player.y - turret.y) * (player.y - turret.y));
}

// reset the bullet method
void WatcherLight::Reset()
{
	// reset the x position off the screen
	posRect.x = -3000;

	// update the pos_X for precision
	pos_X = posRect.x;

	// deactivate the bullet
	active = false;
}

// bullet draw method
void WatcherLight::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

// bullet update method
void WatcherLight::Update(float deltaTime)
{

	// get direction values to move with
	float vX = ((player.x - 50) - turret.x) / distance;
	float vY = ((player.y - 50) - turret.y) / distance;

	// increment the bullet position by vX and vY each frame
	pos_X += (vX * speed * deltaTime);
	pos_Y += (vY * speed * deltaTime);

	// update the bullet position with code to account for precision loss
	posRect.x = (int)(pos_X + .5f);
	posRect.y = (int)(pos_Y + .5f);

	// check to see if the bullet is off the top of the screen
	// and deactivate and move off screen
	if ((posRect.y < (0 - posRect.h)) || (posRect.y > 768) || (posRect.x < (0 - posRect.w)) || (posRect.x > 1024))
	{
		Reset();
	}
}

// tank move the Watcher in X
void WatcherLight::LightMoveX(float tankSpeed, float deltaTime)
{
	pos_X += (tankSpeed)* deltaTime;

	// update the bullet position with code to account for precision loss
	posRect.x = (int)(pos_X + .5f);
}

// tank move the Watcher in Y
void WatcherLight::LightMoveY(float tankSpeed, float deltaTime)
{
	pos_Y += (tankSpeed)* deltaTime;
	
	// update the bullet position with code to account for precision loss
	posRect.y = (int)(pos_Y + .5f);
}