#include "tankBullet.h"

// bullet creation method
Rocks::Rocks(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY)
{
	// set the bullet initial state
	active = false;

	// set bullet speed
	speed = 300;

	// set stop to false
	stop = false;

	hit = false;

	// set explode bool to false
	explode = false;

	// create the texture from the passed renderer and created surface
	texture = IMG_LoadTexture(renderer, filePath.c_str());

	// set the width and height of the bullet's rect
	int w, h;
	//SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = 20;
	posRect.h = 20;

	// set the x and y pos of the bullet rect
	// finish aligning to the player center using the texture width
	posRect.x = x - (posRect.w / 2);
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
void Rocks::Reset()
{
	// reset the x position off the screen
	posRect.x = -1000;

	// update the pos+X for precision
	pos_X = posRect.x;

	// deactivate the bullet
	active = false;
	stop = false;
	explode = false;
	hit = false;
}

// bullet draw method
void Rocks::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

// bullet update method
void Rocks::Update(float deltaTime)
{
	// adjust position floats based on speed, direction (-1 for up), and deltaTime
	//pos_X += (speed * xDir) * deltaTime;
	//pos_Y += (speed * yDir) * deltaTime;
	if (!stop) {
		float radians = (tankAngle * 3.14) / 180;

		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		pos_X += (move_x)* deltaTime;
		pos_Y += (move_y)* deltaTime;

		// update bullet position with code to account for precision loss
		posRect.x = (int)(pos_X + .5f);
		posRect.y = (int)(pos_Y + .5f);
	}

	distance = Distance(sX, sY, pos_X, pos_Y);

	if (distance > 300 && !stop || hit) {
		stop = true;
		posB_X = posRect.x;
		posB_Y = posRect.y;
		explode = true;
	}

	// check to see if the bullet is off the top of the screen
	// and deactivate and move off screen
	if ((posRect.y < (0 - posRect.h)) || (posRect.y > 768) || (posRect.x < (0 - posRect.w)) || (posRect.x > 1024))
	{
		Reset();
	}
}

float Rocks::Distance(float x1, float y1, float x2, float y2)
{
	float x, y, sq;
	x = x1 - x2;
	y = y1 - y2;

	sq = (x * x) + (y * y);

	return sqrt(sq);
}

// tank move the Building in X
void Rocks::TankMoveX(float tankSpeed, float deltaTime)
{
	posB_X += (tankSpeed)* deltaTime;
	//posT_X += (tankSpeed)* deltaTime;

	// update the bullet position with code to account for precision loss
	posRect.x = (int)(posB_X + .5f);
	//barrelRect.x = (int) (posT_X + .5f);
}

// tank move the Building in Y
void Rocks::TankMoveY(float tankSpeed, float deltaTime)
{
	posB_Y += (tankSpeed)* deltaTime;
	//posT_Y += (tankSpeed)* deltaTime;

	// update the bullet position with code to account for precision loss
	posRect.y = (int)(posB_Y + .5f);
	//barrelRect.y = (int) (posT_Y + .5f);
}
