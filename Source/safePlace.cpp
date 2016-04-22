#include "safePlace.h"

Safe::Safe(SDL_Renderer *renderer, string filePath, float x, float y)
{
	active = true;

	string basePath = filePath + "safePlace.png";

	safe = IMG_LoadTexture(renderer, basePath.c_str());

	safeRect.x = x;
	safeRect.y = y;

	sRect.x = x;
	sRect.y = y;

	int w, h;
	SDL_QueryTexture(safe, NULL, NULL, &w, &h);
	safeRect.w = w / 3;
	safeRect.h = h / 3;

	posJ_X = safeRect.x;
	posJ_Y = safeRect.y;
}

void Safe::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, safe, NULL, &safeRect);
}

void Safe::Update(float deltaTime)
{

}

void Safe::Reset()
{
	active = true;
	safeRect.x = posJ_X = sRect.x;
	safeRect.y = posJ_Y = sRect.y;
}

void Safe::TankMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed)* deltaTime;

	safeRect.x = (int)(posJ_X + 0.5f);
}

void Safe::TankMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed)* deltaTime;

	safeRect.y = (int)(posJ_Y + 0.5f);
}
