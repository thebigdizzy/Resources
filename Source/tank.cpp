#include "tank.h"
#include "rockHit.h"

// analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

vector<RockHit> rockHitList;

void rockHits(int x, int y)
{
	// see if there is an explosion not active to use
	for (int i = 0; i < rockHitList.size(); i++)
	{
		// see if the explosion is not active
		if (rockHitList[i].active == false)
		{
			// set explosion to active
			rockHitList[i].active = true;

			// use some math in the x position to get the bullet close to
			// the center of the player using player width
			rockHitList[i].posRect.x = x;
			rockHitList[i].posRect.y = y;

			// once explosion is found, break out of loop
			break;
		}
	}
}

// tank creation
Tank::Tank(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y)
{
	// load the health GUI
	back = IMG_LoadTexture(renderer, (filePath + "health_1.png").c_str());
	mid = IMG_LoadTexture(renderer, (filePath + "health_2.png").c_str());
	front = IMG_LoadTexture(renderer, (filePath + "health_3.png").c_str());

	empty = IMG_LoadTexture(renderer, (filePath + "emptyKeyGUI.png").c_str());
	one = IMG_LoadTexture(renderer, (filePath + "oneKeyGUI.png").c_str());
	two = IMG_LoadTexture(renderer, (filePath + "twoKeyGUI.png").c_str());

	none = IMG_LoadTexture(renderer, (filePath + "tLegNoneGUI.png").c_str());
	One = IMG_LoadTexture(renderer, (filePath + "tLegOneGUI.png").c_str());
	Two = IMG_LoadTexture(renderer, (filePath + "tLegTwoGUI.png").c_str());
	Three = IMG_LoadTexture(renderer, (filePath + "tLegThreeGUI.png").c_str());

	// bring in the rock landing sound
	// sound effect for the rock hit
	//rockLand = Mix_LoadWAV((audioPath + "rockClatter.wav").c_str());

	// set win condition to false
	win1 = false;
	win2 = false;

	key = 0;

	emptyR.x = oneR.x = twoR.x = 10;
	emptyR.y = oneR.y = twoR.y = 10;
	emptyR.w = oneR.w = twoR.w = 100;
	emptyR.h = oneR.h = twoR.h = 100;

	noneR.x = OneR.x = TwoR.x = ThreeR.x = 800;
	noneR.y = OneR.y = TwoR.y = ThreeR.y = 10;
	noneR.w = OneR.w = TwoR.w = ThreeR.w = 200;
	noneR.h = OneR.h = TwoR.h = ThreeR.h = 100;
	
	backR.x = midR.x = frontR.x = 350;
	backR.y = midR.y = frontR.y = 10;
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

	lockX = false;
	lockY = false;

	// create a pool of explosions - 10
	for (int i = 0; i < 10; i++) {
		// create the enemy
		RockHit tmpExplode(renderer, filePath, -1000, -1000, 0);

		// add to the enemyList
		rockHitList.push_back(tmpExplode);
	}

	// create the rects for the rock ammo indicators
	oneRock.w = twoRock.w = threeRock.w = 80;
	oneRock.h = twoRock.h = threeRock.h = 80;

	oneRock.x = 10;
	twoRock.x = 90;
	threeRock.x = 170;

	oneRock.y = twoRock.y = threeRock.y = 700;

}

void Tank::eTankHit()
{
	playerHealth -= .005f;

	//midR.w = playerHealth / maxHealth * 300;
}

void Tank::eBulletHit()
{
	playerHealth -= 5;

	//midR.w = playerHealth / maxHealth * 300;
}

void Tank::Update(float deltaTime)
{
	if (tLegNum > 2 && key) {
		win1 = true;
	}
	else {
		win1 = false;
	}

	if (tLegNum > 2 && key > 1) {
		win2 = true;
	}
	else {
		win2 = false;
	}

	// set the rock hit explosion here
	for (int i = 0; i < bulletList.size(); i++)
	{
		int x = bulletList[i].posRect.x - (bulletList[i].posRect.w + 10);
		int y = bulletList[i].posRect.y - (bulletList[i].posRect.h + 10);

		if (bulletList[i].explode && bulletList[i].stop) {
			rockHits(x, y);
			bulletList[i].explode = false;
			//Mix_PlayChannel(-1, rockLand, 0);
		}
	}

	// update the player's health
	playerHealth -= .5 * deltaTime;

	// update the player's health GUI
	midR.w = playerHealth / maxHealth * 300;

	// update the explosions if they are active
	for (int i = 0; i < rockHitList.size(); i++)
	{
		if (rockHitList[i].active) {
			rockHitList[i].Update(deltaTime);
		}
	}

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

	if (posRect.x > 511) {
		lockX = true;
	}
	if (posRect.y > 383) {
		lockY = true;
	}

	//pos_X += (speed *xDir) * deltaTime;
	//pos_Y += (speed *yDir) * deltaTime;

	// check if the tank is off screen and set it back
	if (lockX) {
		if (posRect.x < 511)
		{
			posRect.x = 511;
			pos_X = posRect.x;
		}

		if (posRect.x > 513) {
			posRect.x = 513;
			pos_X = posRect.x;
		}
	}
	if (lockY) {
		if (posRect.y < 383)
		{
			posRect.y = 383;
			pos_Y = posRect.y;
		}

		if (posRect.y > 385)
		{
			posRect.y = 385;
			pos_Y = posRect.y;
		}
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

		// pickup the rocks
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

	// draw the rock GUIs
	if (rocks == 1) {
		SDL_RenderCopy(renderer, bulletList[9].texture, NULL, &oneRock);
	}
	else if (rocks == 2) {
		SDL_RenderCopy(renderer, bulletList[9].texture, NULL, &oneRock);
		SDL_RenderCopy(renderer, bulletList[9].texture, NULL, &twoRock);
	}
	else if (rocks >= 3) {
		rocks = 3;
		SDL_RenderCopy(renderer, bulletList[9].texture, NULL, &oneRock);
		SDL_RenderCopy(renderer, bulletList[9].texture, NULL, &twoRock);
		SDL_RenderCopy(renderer, bulletList[9].texture, NULL, &threeRock);
	}

	// draw the explosions if they are active
	for (int i = 0; i < rockHitList.size(); i++)
	{
		if (rockHitList[i].active) {
			rockHitList[i].Draw(renderer);
		}
	}

	// draw the player's bullets
	for (int i = 0; i < bulletList.size(); i++)
	{
		// check to see if the bullet is active
		if (bulletList[i].active) {
			// draw the bullet
			bulletList[i].Draw(renderer);
		}
	}

	// draw the health bar
	SDL_RenderCopy(renderer, back, NULL, &backR);
	SDL_RenderCopy(renderer, mid, NULL, &midR);
	SDL_RenderCopy(renderer, front, NULL, &frontR);

	// draw the key stuff
	if (key == 0) {
		SDL_RenderCopy(renderer, empty, NULL, &emptyR);
	}
	else if (key == 1) {
		SDL_RenderCopy(renderer, one, NULL, &oneR);
	}
	else if (key == 2) {
		SDL_RenderCopy(renderer, two, NULL, &twoR);
	}
	else {
		SDL_RenderCopy(renderer, empty, NULL, &emptyR);
	}

	// draw the turkey stuff
	if (tLegNum == 0) {
		SDL_RenderCopy(renderer, none, NULL, &noneR);
	}
	else if (tLegNum == 1) {
		SDL_RenderCopy(renderer, One, NULL, &OneR);
	}
	else if (tLegNum == 2) {
		SDL_RenderCopy(renderer, Two, NULL, &TwoR);
	}
	else if (tLegNum == 3) {
		SDL_RenderCopy(renderer, Three, NULL, &ThreeR);
	}
	else {
		SDL_RenderCopy(renderer, none, NULL, &noneR);
	}
}

void Tank::Reset()
{
	posRect.x = 150;
	posRect.y = 150;
	lockX = false;
	lockY = false;
	pos_X = 150;
	pos_Y = 150;
	rocks = 0;
	tLegNum = 0;
	key = 0;
	win1 = false;
	win2 = false;
	playerHealth = 100;

	for (int i = 0; i < bulletList.size(); i++)
	{
		bulletList[i].active = false;
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

int Tank::lightCollision(SDL_Rect light)
{
	if (SDL_HasIntersection(&light, &posRect)) {
		return 1;
	}

	return 0;
}

int Tank::turkeyCollision(SDL_Rect tLegRect)
{
	if (SDL_HasIntersection(&tLegRect, &posRect)) {
		tLegNum++;
		return 1;
	}

	return 0;
}

int Tank::rockCollision(SDL_Rect rockRect)
{
	if (SDL_HasIntersection(&rockRect, &posRect)) {
		rocks++;
		return 1;
	}

	return 0;
}

int Tank::keyCollision(SDL_Rect keyRect)
{
	if (SDL_HasIntersection(&keyRect, &posRect)) {
		key++;
		return 1;
	}

	return 0;
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


