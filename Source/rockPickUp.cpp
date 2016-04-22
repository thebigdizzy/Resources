#include "rockPickUp.h"

Rock::Rock(SDL_Renderer *renderer, string filePath, float x, float y)
{
	active = true;

	string basePath = filePath + "rockitem.png";

	rock = IMG_LoadTexture(renderer, basePath.c_str());

	rockRect.x = x;
	rockRect.y = y;

	sRect.x = x;
	sRect.y = y;

	int w, h;
	SDL_QueryTexture(rock, NULL, NULL, &w, &h);
	rockRect.w = 20;
	rockRect.h = 20;

	posJ_X = rockRect.x;
	posJ_Y = rockRect.y;
}

void Rock::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, rock, NULL, &rockRect);
}

void Rock::Update(float deltaTime)
{

}

void Rock::Reset()
{
	active = true;
	rockRect.x = posJ_X = sRect.x;
	rockRect.y = posJ_Y = sRect.y;
}

void Rock::TankMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed)* deltaTime;

	rockRect.x = (int)(posJ_X + 0.5f);
}

void Rock::TankMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed)* deltaTime;

	rockRect.y = (int)(posJ_Y + 0.5f);
}
