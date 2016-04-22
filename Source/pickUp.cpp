#include "pickUp.h"

Food::Food(SDL_Renderer *renderer, string filePath, float x, float y)
{
	active = true;

	string basePath = filePath + "turkeyLeg.png";

	tLeg = IMG_LoadTexture(renderer, basePath.c_str());

	tLegRect.x = x;
	tLegRect.y = y;

	sRect.x = x;
	sRect.y = y;

	int w, h;
	SDL_QueryTexture(tLeg, NULL, NULL, &w, &h);
	tLegRect.w = w/3;
	tLegRect.h = h/3;

	posJ_X = tLegRect.x;
	posJ_Y = tLegRect.y;
}

void Food::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, tLeg, NULL, &tLegRect);
}

void Food::Update(float deltaTime)
{

}

void Food::Reset()
{
	active = true;
	tLegRect.x = posJ_X = sRect.x;
	tLegRect.y = posJ_Y = sRect.y;
}

void Food::TankMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed)* deltaTime;

	tLegRect.x = (int)(posJ_X + 0.5f);
}

void Food::TankMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed)* deltaTime;

	tLegRect.y = (int)(posJ_Y + 0.5f);
}
