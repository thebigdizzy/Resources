#include "watcher.h"

// Rurret creation method
Watcher::Watcher(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	// activate the Watcher
	active = true;

	// fire sound
	//fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	// create the Watcher base file path
	string basePath = filePath + "watcherStand.png";

	// load the image into the texture
	tBase = IMG_LoadTexture(renderer, basePath.c_str());

	// create the Watcherbase file path
	string barrelPath = filePath + "Watcher.png";

	// load the image into the texture
	tBarrel = IMG_LoadTexture(renderer, barrelPath.c_str());

	// set the SDL_Rect x and y for the base image
	baseRect.x = x;
	baseRect.y = y;

	// use SDL_QueryTexture to get the w and h of the base's texture
	int w, h;
	SDL_QueryTexture(tBase, NULL, NULL, &w, &h);
	baseRect.w = w/3;
	baseRect.h = h/3;

	// set the SDL_Rect x and y for the barrel
	barrelRect.x = x;
	barrelRect.y = y;

	// use the SDL_QueryTexture to get the w and h of the barrel's texture
	SDL_QueryTexture(tBarrel, NULL, NULL, &w, &h);
	barrelRect.w = w/3;
	barrelRect.h = h/3;

	// create the center point of the barrel texture for rotation
	center.x = barrelRect.w/2;
	center.y = barrelRect.h/2;

	// string to create the path to the player's bullet image
	string bulletPath = filePath + "watcherLight.png";

	// create the Watcher's bullet pool
	for (int i = 0; i < 10; i++)
	{
		// create the bullet and move offscreen, out of the gameplay area
		WatcherLight tmpBullet(renderer, bulletPath, -3000, 3000);

		// add to the lightList
		lightList.push_back(tmpBullet);
	}

	// random null init
	srand(time(NULL));

	// update the float values for movement
	posB_X = baseRect.x;
	posB_Y = baseRect.y;

	posT_X = barrelRect.x;
	posT_Y = barrelRect.y;

	WatcherAngle = 0;
}

// tank move the Watcher in X
void Watcher::TankMoveX(float tankSpeed, float deltaTime)
{
	posB_X += (tankSpeed) * deltaTime;
	posT_X += (tankSpeed) * deltaTime;

	// update the bullet position with code to account for precision loss
	baseRect.x = (int) (posB_X + .5f);
	barrelRect.x = (int) (posT_X + .5f);

	for (int i = 0; i < lightList.size(); i++)
	{
		lightList[i].LightMoveX(tankSpeed, deltaTime);
	}
}

// tank move the Watcher in Y
void Watcher::TankMoveY(float tankSpeed, float deltaTime)
{
	posB_Y += (tankSpeed) * deltaTime;
	posT_Y += (tankSpeed) * deltaTime;

	// update the bullet position with code to account for precision loss
	baseRect.y = (int) (posB_Y + .5f);
	barrelRect.y = (int) (posT_Y + .5f);

	for (int i = 0; i < lightList.size(); i++)
	{
		lightList[i].LightMoveY(tankSpeed, deltaTime);
	}
}

void Watcher::Draw(SDL_Renderer *renderer)
{
	// draw the Watcher's bullet
	for (int i = 0; i < lightList.size(); i++)
	{
		// check to see if the bullet is active
		if(lightList[i].active)
		{
			// draw tthe bullet
			lightList[i].Draw(renderer);
		}
	}

	// draw the base
	SDL_RenderCopy(renderer, tBase, NULL, &baseRect);

	// draw the barrel texture using the vars texture and posRect
	SDL_RenderCopyEx(renderer, tBarrel, NULL, &barrelRect, WatcherAngle, &center, SDL_FLIP_NONE);
}

// tank update method
void Watcher::Update(float deltaTime, SDL_Rect tankRect)
{
	// get the angle between the tank and the Watcher
	x = (tankRect.x + (tankRect.w/2)) - (baseRect.x + (baseRect.w/2));
	y = (tankRect.y + (tankRect.h/2)) - (baseRect.y + (baseRect.h/2));
	static bool lR = false;
	static bool Rl = false;
	if(WatcherAngle <= -180){
		lR = true;
		Rl = false;
	} else if(WatcherAngle >= 0){
		lR = false;
		Rl = true;
	}

	if(lR)
		WatcherAngle+= .1f;
	else if(Rl)
		WatcherAngle-= .1f;

	cout << WatcherAngle << endl;

	if(SDL_GetTicks() > fireTime)
	{
		if(baseRect.x > 0 && baseRect.x < 1024 && baseRect.y > 0 && baseRect.y < 768){
			CreateBullet(tankRect);
		}

		fireTime = SDL_GetTicks() + (rand() % 3 + 1) * 3000;
	}

	// update the Watcher's bullets
	for (int i = 0; i < lightList.size(); i++)
		{
			// check to see if the bullet is active
			if(lightList[i].active)
			{
				// draw tthe bullet
				lightList[i].Update(deltaTime, baseRect);
			}
		}
}

// create the bullet
void Watcher::CreateBullet(SDL_Rect target)
{
	// see if there is a bullet active to fire
	for (int i = 0; i < lightList.size(); i++)
	{
		// see if the bulelt is active
		if(lightList[i].active == false)
		{
			lightList[i].Start(WatcherAngle);

			// player the over sound = plays fine through levels, must pause for Quit
			//Mix_PlayChannel(-1, fire, 0);

			// set the bullet to active
			lightList[i].active = true;

			// use some math in the x pos to get the bullet close to
			// the center of the Watcher
			lightList[i].posRect.x = ((baseRect.x + (baseRect.w/2)) - (lightList[i].posRect.w/2));
			lightList[i].posRect.y = ((baseRect.y + (baseRect.h/2)) - (lightList[i].posRect.h/2));

			// set the x and y pos of the bullet's float pos
			lightList[i].pos_X = lightList[i].posRect.x;
			lightList[i].pos_Y = lightList[i].posRect.y;

			// once the bullet is found break out of the loop
			break;

		}
	}
}

