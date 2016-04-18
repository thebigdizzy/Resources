#include "tank.h"

// analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

// tank creation
Tank::Tank(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y)
{
	// load the health GUI
	//back = IMG_LoadTexture(renderer, (filePath + "health_1.png").c_str());
	//mid = IMG_LoadTexture(renderer, (filePath + "health_2.png").c_str());
	//front = IMG_LoadTexture(renderer, (filePath + "health_3.png").c_str());
	
	backR.x = midR.x = frontR.x = 10;
	backR.y = midR.y = frontR.y = 50;
	backR.w = midR.w = frontR.w = 300;
	backR.h = midR.h = frontR.h = 60;

	// player health
	playerHealth = 100.0f;
	maxHealth = 100.0f;

	active = true;

	playerNum = pNum;

	speed = 100.0f;

	// number of rocks Bibble starts with
	rocks = 3;

	// tank firing sound
	//fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	playerPath = filePath + "Bibble.png";

	texture = IMG_LoadTexture(renderer, playerPath.c_str());

	if(texture == NULL)
	{
		printf("Could not get image: %s\n", SDL_GetError());
	}

	posRect.x = x;
	posRect.y = y;
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	pos_X = x;
	pos_Y = y;

	xDir = 0;
	yDir = 0;

	xDirOld = 1;
	yDirOld = 0;

	center.x = posRect.w/2;
	center.y = posRect.h/2;

	string bulletPath;

	if(playerNum == 0)
	{
		bulletPath = filePath + "rockItem.png";
	} 

	//create the player's bullet pool
	for (int i = 0; i < 10; i++)
	{
		//create the bullet and move offscreen, out of the gameplayer area
		Rocks tmpBullet(renderer, bulletPath, -1000, -1000, 0, 0);

		//add to bulletList
		bulletList.push_back(tmpBullet);
	}
}

void Tank::eTankHit()
{
	playerHealth -= .005f;

	midR.w = playerHealth / maxHealth * 300;
}

void Tank::eBulletHit()
{
	playerHealth -= 5;

	midR.w = playerHealth / maxHealth * 300;
}

void Tank::Update(float deltaTime)
{
	// check for gamepad input
	if(Xvalue != 0 || Yvalue != 0)
	{
	//x = posRect.x - xDir;
	//y = posRect.y - yDir;
	tankAngle = atan2(Yvalue, Xvalue) * 180/3.14f;
	// set this as the old angle and dir some the player/tank can shoot when stopped
	oldAngle = tankAngle;
	//xDirOld = xDir;
	//yDirOld = yDir;

	// gives us radians
	float radians = (tankAngle * 3.14f)/180;

	float move_x = speed * cos(radians);
	float move_y = speed * sin(radians);

	//  update floats for precision loss
	pos_X += (move_x) * deltaTime;
	pos_Y += (move_y) * deltaTime;

	// update floats for precision loss
	posRect.x = (int)(pos_X + .5f);
	posRect.y = (int)(pos_Y + .5f);

	} else
	{
		tankAngle = oldAngle;
	}

	//pos_X += (speed *xDir) * deltaTime;
	//pos_Y += (speed *yDir) * deltaTime;

	// check if the tank is off screen and set it back
	if(posRect.x < 511)
	{
		posRect.x = 511;
		pos_X = posRect.x;
	}

	if(posRect.x > 513){
		posRect.x = 513;
		pos_X = posRect.x;
	}

	if(posRect.y < 383)
	{
		posRect.y = 383;
		pos_Y = posRect.y;
	}

	if(posRect.y > 385)
	{
		posRect.y = 385;
		pos_Y = posRect.y;
	}

		// update the tank's bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			// check to see if the bullet is active
			if(bulletList[i].active){
				// update the bullet
				bulletList[i].Update(deltaTime);
			}
		}

		// update the rocks
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active && bulletList[i].stop) {
				if (SDL_HasIntersection(&posRect, &bulletList[i].posRect))
				{
					bulletList[i].Reset();
					// pickup the rock
					rocks++;
				}
			}
		}
}

void Tank::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, tankAngle, &center, SDL_FLIP_NONE);
	//SDL_RenderCopy(renderer, back, NULL, &backR);
	//SDL_RenderCopy(renderer, mid, NULL, &midR);
	//SDL_RenderCopy(renderer, front, NULL, &frontR);

	// draw the player's bullets
	for (int i = 0; i < bulletList.size(); i++)
	{
		// check to see if the bullet is active
		if(bulletList[i].active){
			// draw the bullet
			bulletList[i].Draw(renderer);
		}
	}

}

void Tank::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	// if the player's number is 0 and the joystick buttom is from joystick 0
	if(event.which == 0 && playerNum == 0)
	{
		// if A button
		if(event.button == 0)
		{
			// create a bullet
			CreateBullet();
		}
	}
}

void Tank::OnControllerAxis(Sint16 X, Sint16 Y)
{
	/*if(event.which == 0 && playerNum == 0)
	{
		// x axis
		if(event.axis == 0)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f;
			}else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f;
			} else {
				xDir = 0.0f;
			}
		}
		// y axis
		if(event.axis == 1)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f;
			} else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f;
			} else {
				yDir = 0.0f;
			}
		}
	}

	if(event.which == 1 && playerNum == 1)
		{
			// x axis
			if(event.axis == 0)
			{
				if(event.value < -JOYSTICK_DEAD_ZONE)
				{
					xDir = -1.0f;
				}else if (event.value > JOYSTICK_DEAD_ZONE)
				{
					xDir = 1.0f;
				} else {
					xDir = 0.0f;
				}
			}
			// y axis
			if(event.axis == 1)
			{
				if(event.value < -JOYSTICK_DEAD_ZONE)
				{
					yDir = -1.0f;
				} else if(event.value > JOYSTICK_DEAD_ZONE)
				{
					yDir = 1.0f;
				} else {
					yDir = 0.0f;
				}
			}
		}*/

	Xvalue = X;
	Yvalue = Y;

	if(!(Xvalue < -JOYSTICK_DEAD_ZONE) && !(Xvalue > JOYSTICK_DEAD_ZONE))
	{
		Xvalue = 0.0f; // NONE
	}

	if(!(Yvalue < -JOYSTICK_DEAD_ZONE) && !(Yvalue > JOYSTICK_DEAD_ZONE))
		{
			Yvalue = 0.0f; // NONE
		}
}

// create bullet method
void Tank::CreateBullet()
{
	// see if there is a bullet active to fire
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (rocks) {
			// see if the bullet is not active
			if (bulletList[i].active == false)
			{
				rocks--;
				// player the over sound - players fine through levels, must pause for QUIT
				//Mix_PlayChannel(-1, fire, 0);

				// set bulelt to active
				bulletList[i].active = true;

				// use some math in the x position to get the bullet close to
				// the center of the player using player width
				bulletList[i].posRect.x = (posRect.x + (posRect.w / 2));
				bulletList[i].posRect.y = (posRect.y + (posRect.h / 2));

				// finishing alighning to the player center using the texture width
				bulletList[i].posRect.x = bulletList[i].posRect.x - (bulletList[i].posRect.w / 2);
				bulletList[i].posRect.y = bulletList[i].posRect.y - (bulletList[i].posRect.h / 2);

				// set the x and y position of the bullet's float positions
				bulletList[i].pos_X = bulletList[i].posRect.x;
				bulletList[i].pos_Y = bulletList[i].posRect.y;

				// if the tank is moving fire in the direction
				if (Xvalue != 0 || Yvalue != 0)
				{
					// set the x and y positions of the bullet's float position
					bulletList[i].tankAngle = tankAngle;
					//bulletList[i].yDir = yDir;
				}
				else {
					// if the tank is not moving, fire in the direction currently facing
					// set the x and y positions of the bullet's float positions
					bulletList[i].tankAngle = oldAngle;
					//bulletList[i].yDir = yDirOld;
				}

				// set the starting point of the bullet
				bulletList[i].sX = bulletList[i].posRect.x;
				bulletList[i].sY = bulletList[i].posRect.y;

				// once bullet is four , break out of loop
				break;

			}
		}
	}
}


