#include "watcherLight.h"

// bullet cretion method
WatcherLight::WatcherLight(SDL_Renderer * renderer, string filePath, float x, float y)
{
	// set the bullet initial state
	active = false;

	// set the bullet speed
	speed = 10.0f;

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
	initX = x;
	initY = y;

	comeBack = false;
}

// reset the bullet method
void WatcherLight::Start(float angle)
{
	// initialize the angle
	startAngle = angle;
}

// reset the bullet method
void WatcherLight::Reset()
{
	// reset the x position off the screen
	posRect.x = -3000;

	// update the pos_X for precision
	pos_X = initX = posRect.x;

	// deactivate the bullet
	active = false;
	comeBack = false;
}

// bullet draw method
void WatcherLight::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

// bullet update method
void WatcherLight::Update(float deltaTime, SDL_Rect watcher)
{
	turret.x = watcher.x;
	turret.y = watcher.y;
	turret.w = watcher.w;
	turret.h = watcher.h;

	float distance1 = sqrt((turret.x - posRect.x) * (turret.x - posRect.x) + (turret.y - posRect.y) * (turret.y - posRect.y));

	//cout << distance1 << endl;

	if(distance1 > 300){
		comeBack = true;
	}

	if(!comeBack){
		// get direction values to move with
		//float vX = ((player.x - 50) - turret.x) / distance;
		//float vY = ((player.y - 50) - turret.y) / distance;

		float radians = (startAngle * 3.14) / 180;

		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		// increment the bullet position by vX and vY each frame
		pos_X += (move_x * speed * deltaTime);
		pos_Y += (move_y * speed * deltaTime);

		// update the bullet position with code to account for precision loss
		posRect.x = (int)(pos_X + .5f);
		posRect.y = (int)(pos_Y + .5f);
	} else {
		// get direction values to move with
		//float vX = ((player.x - 50) - turret.x) / distance;
		//float vY = ((player.y - 50) - turret.y) / distance;

		float radians = (startAngle * 3.14) / 180;

		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		// increment the bullet position by vX and vY each frame
		pos_X -= (move_x * speed * deltaTime);
		pos_Y -= (move_y * speed * deltaTime);

		// update the bullet position with code to account for precision loss
		posRect.x = (int)(pos_X + .5f);
		posRect.y = (int)(pos_Y + .5f);

		if(SDL_HasIntersection(&turret, &posRect)){
			Reset();
		}
	}

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
