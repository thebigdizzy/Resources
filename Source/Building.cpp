#include "Building.h"

// Rurret creation method
Turret::Turret(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	// activate the turret
	active = true;

	// fire sound
	//fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	// create the turret base file path
	string basePath = filePath + "building1.png";

	// load the image into the texture
	tBase = IMG_LoadTexture(renderer, basePath.c_str());

	// create the turretbase file path
	//string barrelPath = filePath + "tBarrel.png";

	// load the image into the texture
	//tBarrel = IMG_LoadTexture(renderer, barrelPath.c_str());

	// set the SDL)Rect x and y for the base image
	baseRect.x = x;
	baseRect.y = y;

	// use SDL_QueryTexture to get the w and h of the base's texture
	int w, h;
	SDL_QueryTexture(tBase, NULL, NULL, &w, &h);
	baseRect.w = w;
	baseRect.h = h;

	// set the SDL_Rect x and y for the barrel
	barrelRect.x = x + 35;
	barrelRect.y = y + 35;

	// use the SDL_QueryTexture to get the w and h of the barrel's texture
	/*SDL_QueryTexture(tBarrel, NULL, NULL, &w, &h);
	barrelRect.w = w;
	barrelRect.h = h;*/

	// create the center point of the barrel texture for rotation
	center.x = 13;
	center.y = 13;

	// string to create the path to the player's bullet image
	//string bulletPath = filePath + "tBullet.png";

	// create the turret's bullet pool
	//for (int i = 0; i < 10; i++)
	//{
	//	// create the bullet and move offscreen, out of the gameplay area
	//	//TurretBullet tmpBullet(renderer, bulletPath, 1500, 1500);

	//	// add to the bulletList
	//	bulletList.push_back(tmpBullet);
	//}

	// random null init
	//srand(time(NULL));

	// update the float values for movement
	posB_X = baseRect.x;
	posB_Y = baseRect.y;

	/*posT_X = barrelRect.x;
	posT_Y = barrelRect.y;*/

}

// tank move the turret in X
void Turret::TankMoveX(float tankSpeed, float deltaTime)
{
	posB_X += (tankSpeed) * deltaTime;
	posT_X += (tankSpeed) * deltaTime;

	// update the bullet position with code to account for precision loss
	baseRect.x = (int) (posB_X + .5f);
	//barrelRect.x = (int) (posT_X + .5f);
}

// tank move the turret in Y
void Turret::TankMoveY(float tankSpeed, float deltaTime)
{
	posB_Y += (tankSpeed) * deltaTime;
	posT_Y += (tankSpeed) * deltaTime;

	// update the bullet position with code to account for precision loss
	baseRect.y = (int) (posB_Y + .5f);
	//barrelRect.y = (int) (posT_Y + .5f);
}

void Turret::Draw(SDL_Renderer *renderer)
{
	// draw the turret's bullet
	//for (int i = 0; i < bulletList.size(); i++)
	//{
	//	// check to see if the bullet is active
	//	if(bulletList[i].active)
	//	{
	//		// draw tthe bullet
	//		bulletList[i].Draw(renderer);
	//	}
	//}

	// draw the base
	SDL_RenderCopy(renderer, tBase, NULL, &baseRect);

	// draw the barrel texture using the vars texture and posRect
	//SDL_RenderCopyEx(renderer, tBarrel, NULL, &barrelRect, turretAngle, &center, SDL_FLIP_NONE);
}

// tank update method
void Turret::Update(float deltaTime, SDL_Rect tankRect)
{
	// get the angle between the tank and the turret
	x = (tankRect.x + (tankRect.w/2)) - (baseRect.x + (baseRect.w/2));
	y = (tankRect.y + (tankRect.h/2)) - (baseRect.y + (baseRect.h/2));
	//turretAngle = atan2(y,x) * 180 / 3.14f;

	/*if(SDL_GetTicks() > fireTime)
	{
		if(baseRect.x > 0 && baseRect.x < 1024 && baseRect.y > 0 && baseRect.y < 768){
			CreateBullet(tankRect);
		}

		fireTime = SDL_GetTicks() + (rand() % 3 + 1) * 1000;
	}*/

	// update the turret's bullets
	//for (int i = 0; i < bulletList.size(); i++)
	//	{
	//		// check to see if the bullet is active
	//		if(bulletList[i].active)
	//		{
	//			// draw tthe bullet
	//			bulletList[i].Update(deltaTime);
	//		}
	//	}
}

// create the bullet
void Turret::CreateBullet(SDL_Rect target)
{
	
	//// see if there is a bullet active to fire
	//for (int i = 0; i < bulletList.size(); i++)
	//{
	//	// see if the bulelt is active
	//	if(bulletList[i].active == false)
	//	{
	//		bulletList[i].Start(target, baseRect);

	//		// player the over sound = plays fine through levels, must pause for Quit
	//		//Mix_PlayChannel(-1, fire, 0);

	//		// set the bullet to active
	//		bulletList[i].active = true;

	//		// use some math in the x pos to get the bulelt clos to
	//		// the center of the turret
	//		bulletList[i].posRect.x = ((baseRect.x + (baseRect.w/2)) - (bulletList[i].posRect.w/2));
	//		bulletList[i].posRect.y = ((baseRect.y + (baseRect.h/2)) - (bulletList[i].posRect.h/2));

	//		// set the x and y pos of the bullet's float pos
	//		bulletList[i].pos_X = bulletList[i].posRect.x;
	//		bulletList[i].pos_Y = bulletList[i].posRect.y;

	//		// once the bullet is found break out of the loop
	//		break;

	//	}
	//}
}

