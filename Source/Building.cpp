#include "Building.h"

// Rurret creation method
Building::Building(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y, int id)
{
	// activate the Building
	active = true;

	string basePath = filePath + "building1.png";

	// create the Building base file path
	if(id == 0){
		basePath = filePath + "building1.png";
	} else if(id == 1){
		basePath = filePath + "building1Bot.png";
	}else if(id == 2){
		basePath = filePath + "building1Top.png";
	} else{
		basePath = filePath + "building1.png";
	}

	// load the image into the texture
	tBase = IMG_LoadTexture(renderer, basePath.c_str());

	// set the SDL)Rect x and y for the base image
	baseRect.x = sRect.x = x;
	baseRect.y = sRect.y = y;

	// use SDL_QueryTexture to get the w and h of the base's texture
	int w, h;
	SDL_QueryTexture(tBase, NULL, NULL, &w, &h);
	baseRect.w = w;
	baseRect.h = h;

	// create the center point of the barrel texture for rotation
	center.x = 13;
	center.y = 13;

	// update the float values for movement
	posB_X = sPosX = baseRect.x;
	posB_Y = sPosY = baseRect.y;
}

// tank move the Building in X
void Building::TankMoveX(float tankSpeed, float deltaTime)
{
	posB_X += (tankSpeed) * deltaTime;
	posT_X += (tankSpeed) * deltaTime;

	// update the bullet position with code to account for precision loss
	baseRect.x = (int) (posB_X + .5f);
	//barrelRect.x = (int) (posT_X + .5f);
}

// tank move the Building in Y
void Building::TankMoveY(float tankSpeed, float deltaTime)
{
	posB_Y += (tankSpeed) * deltaTime;
	posT_Y += (tankSpeed) * deltaTime;

	// update the bullet position with code to account for precision loss
	baseRect.y = (int) (posB_Y + .5f);
	//barrelRect.y = (int) (posT_Y + .5f);
}

void Building::Draw(SDL_Renderer *renderer)
{
	// draw the base
	SDL_RenderCopy(renderer, tBase, NULL, &baseRect);
}

void Building::Reset()
{
	posB_X = sPosX;
	posB_Y = sPosY;

	baseRect.x = sRect.x;
	baseRect.y = sRect.y;
}

// tank update method
void Building::Update(float deltaTime, SDL_Rect tankRect)
{
	// get the angle between the tank and the Building
	x = (tankRect.x + (tankRect.w/2)) - (baseRect.x + (baseRect.w/2));
	y = (tankRect.y + (tankRect.h/2)) - (baseRect.y + (baseRect.h/2));

}

